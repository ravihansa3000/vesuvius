<?php

/**
 * Description of facilityImportXLS
 *
 * @author ckramer
 */
class facilityImportXLS
{

  public $importFacilitySpec = array(
    'id' => array('type' => 'integer', 'autoincrement' => true, 'primary' => true),
    'facility_name' => array('type' => "string", 'length' => 64),
    'facility_code' => array('type' => "string", 'length' => 10),
    'facility_resource_type_abbr' => array('type' => "string", 'length' => 10),
    'facility_resource_status' => array('type' => "string", 'length' => 40),
    'facility_capacity' => array('type' => "integer"),
    'facility_activation_sequence' => array('type' => "integer"),
    'facility_allocation_status' => array('type' => "string", 'length' => 30),
    'facility_group' => array('type' => "string", 'length' => 64),
    'facility_group_type' => array('type' => "string", 'length' => 30),
    'facility_group_allocation_status' => array('type' => "string", 'length' => 30),
    'work_email' => array('type' => "string", 'length' => 255),
    'work_phone' => array('type' => "string", 'length' => 32),
    'street_1' => array('type' => "string", 'length' => 255),
    'street_2' => array('type' => "string", 'length' => 255),
    'city' => array('type' => "string", 'length' => 255),
    'state' => array('type' => "string", 'length' => 255),
    'postal_code' => array('type' => "string", 'length' => 30),
    'borough' => array('type' => "string", 'length' => 30),
    'country' => array('type' => "string", 'length' => 10),
    'longitude' => array('type' => "decimal"),
    'latitude' => array('type' => "decimal")
  );
  // Errors go here
  public $events = array();
  public $totalImported = 0;
  public $totalUpdated = 0;
  public $totalNew = 0;
  public $totalErrors = 0;

  /**
   * processFacilityImport()
   *
   * Reads contents of the Excel import file into temp table
   *
   * @param $importFile
   */
  public function processExcel($importFile)
  {
    global $global;
    include_once ($global['approot'] . '/3rd/excelreader/excel_reader2.php');

    $fileInfo = pathinfo($importFile);
    if (strtolower($fileInfo["extension"]) <> 'xls') {
      $this->events[] = array("type" => "ERROR", "message" => "{$fileInfo['basename']} is not Microsoft Excel 2003 \".xls\" workbook.");
    } else {

      $this->events[] = array("type" => "INFO", "message" => "Opening import file for reading.");
      $xlsObj = new Spreadsheet_Excel_Reader($importFile);

      // Get some info about the workbook's composition
      $numSheets = count($xlsObj->sheets);
      $this->events[] = array("type" => "INFO", "message" => "Number of worksheets found: $numSheets");

      $numRows = $xlsObj->rowcount($sheet_index = 0);
      $numCols = $xlsObj->colcount($sheet_index = 0);

      // Create a simplified array from the worksheets
      // Create a simplified array from the worksheets
      $importRow = 0;
      for ($sheet = 0; $sheet < $numSheets; $sheet++) {

        // Get the sheet name
        $sheetName = $xlsObj->boundsheets[$sheet]["name"];
        $this->events[] = array("type" => "INFO", "message" => "Parsing worksheet $sheetName");

        // We don't import sheets named "Validation"
        if (strtolower($sheetName) <> "validation") {
          for ($row = 2; $row <= $numRows; $row++) {

            for ($col = 1; $col <= $numCols; $col++) {

              $colName = str_replace(" ", "_", strtolower($xlsObj->val(1, $col, $sheet)));

              $val = $xlsObj->raw($row, $col, $sheet);
              if (!($val)) {
                $val = $xlsObj->val($row, $col, $sheet);
              }
              $importFileData[$importRow][$colName] = $val;
            }
            // Increment import row
            $importRow++;
          }
        } else {
          $this->events[] = array("type" => "INFO", "message" => "Ignoring $sheetName worksheet");
        }
      }

      $this->events[] = array("type" => "INFO", "message" => "Validating column headers of import file.");

      if ($this->validateColumnHeaders($importFileData)) {

        $this->events[] = array("type" => "OK", "message" => "Valid column headers found.");
        $this->events[] = array("type" => "INFO", "message" => "Saving facilities to database.");
        $this->saveImportData($importFileData);
        $this->events[] = array("type" => "OK", "message" => "Done inserting facility records.");
      } else {

        $this->events[] = array("type" => "ERROR", "message" => "Unable to import file due to validation error.");
      }
    }
  }

  /**
   * saveImportData()
   *
   * Writes import array to temp table
   *
   * @param $importDataSet
   */
  private function saveImportData($importDataSet)
  {
    global $global;

    if (empty($importDataSet)) {
      $this->events[] = array("type" => "ERROR", "message" => "Cannot save empty dataset to temp table.");
      $this->totalErrors++;
    } else {

      // Create an INSERT query for each facility
      foreach ($importDataSet as $row) {

        // Some Excel workbooks create empty rows. Let's ignore those
        $rowString = implode($row);
        if (empty($rowString)) {
          $this->events[] = array("type" => "WARN", "message" => "Ignoring empty row.");
        } else {

          if ($this->checkExisting($row)) {
            // Update record
            $query = $this->makeUpdateStatment($row);
            $this->totalUpdated++;
          } else {
            // Make an INSERT
            $query = $this->makeInsertStatment($row);
            $this->totalNew++;
          }


          // Safe the query
          $query = $global["db"]->Prepare($query);

          try {

            $global['db']->Execute($query);
          } catch (exception $e) {

            $this->events[] = array("type" => "ERROR", "message" => $e->getMessage());
            $this->totalErrors++;
          }
        }
      }

      $this->totalImported = $this->totalNew + $this->totalUpdated;
    }
  }

  private function checkExisting($row)
  {

    global $global;
    $exists = false;

    $query = "SELECT COUNT(*) AS `count` FROM `facility`
              WHERE facility_name = " . shn_db_clean($row["facility_name"]) . "
                  AND facility_code = " . shn_db_clean($row["facility_code"]) . "
                  AND facility_resource_type_abbr = " . shn_db_clean($row["facility_resource_type_abbr"]) . ";";

    try {
      $rs = $global['db']->Execute($query);
    } catch (exception $e) {
      $this->events[] = array("type" => "ERROR", "message" => "<pre>" . $e->getMessage() . "</pre>");
    }

    $results = $rs->FetchRow();

    if ($results["count"] > 0) {
      $exists = true;
    }

    return $exists;
  }

  private function makeUpdateStatment($row)
  {

    // Prepare the INSERT SQL template
    $query = "UPDATE `facility` \nSET %s \nWHERE `facility_name` = %s\n \nAND `facility_code`=%s \nAND `facility_resource_type_abbr`=%s;";
    $updates = "";
    $val = "";

    foreach (array_keys($this->importFacilitySpec) as $column) {

      // Ignore id key name because it is the auto
      if ($column != "id") {

        // Handle null values with sane defaults
        switch ($this->importFacilitySpec[$column]["type"]) {
          case "integer":
            if ($row[$column] == "") {
              $val = 0;
            } else {
              $val = $row[$column];
            };
            break;
          case "decimal":
            if ($row[$column] == "") {
              $val = 0.0;
            } else {
              $val = $row[$column];
            }
            break;
          default:
            $val = trim($row[$column]);
        }

        $update = sprintf("\n\t`%s` = %s,", $column, shn_db_clean($val));
        $updates .= $update;
      }
    }

    // Chop off the trailing comma
    $updates = substr($updates, 0, -1);

    $query = sprintf($query, $updates, shn_db_clean($row["facility_name"]),
                                                    shn_db_clean($row["facility_code"]),
                                                                 shn_db_clean($row["facility_resource_type_abbr"]));

    return $query;
  }

  private function makeInsertStatment($row)
  {

    // Prepare the INSERT SQL template
    $query = "INSERT INTO `facility` (%s) \nVALUES (%s\n);";
    $cols = "";
    $vals = "";

    foreach (array_keys($this->importFacilitySpec) as $column) {

      // Ignore id key name because it is the auto
      if ($column != "id") {
        $col = sprintf("\n\t`%s`,", $column);
        $cols = $cols . $col;

        // Handle null values with sane defaults
        switch ($this->importFacilitySpec[$column]["type"]) {
          case "integer":
            if ($row[$column] == "") {
              $val = 0;
            } else {
              $val = $row[$column];
            };
            break;
          case "decimal":
            if ($row[$column] == "") {
              $val = 0.0;
            } else {
              $val = $row[$column];
            }
            break;
          default:
            $val = trim($row[$column]);
        }

        // Safe the value
        $val = shn_db_clean($val);

        $val = sprintf("\n\t%s,", $val);
        $vals = $vals . "$val";
      }
    }

    // Append creation timestamp
    $cols .= " `creation_time`";
    $vals .= " NOW()";

    $query = sprintf($query, $cols, $vals);

    return $query;
  }

  /**
   * validateColumnHeaders($importFileData)
   *
   * Validates import data for correct schema. Returns bool.
   *
   * @param $importFileData
   */
  private function validateColumnHeaders($importFileData)
  {

    // Cache the import header specification
    $importSpecHeaders = array_keys($this->importFacilitySpec);

    // Check first row for expected column header names
    $importFileHeaders = array_keys(array_shift($importFileData));

    // The import spec will start with an ID column. Shift off of it.
    $idColumn = array_shift($importSpecHeaders);
    $importSpecDiff = array_diff($importSpecHeaders, $importFileHeaders);

    if (empty($importSpecDiff)) {
      return true;
    } else {
      $this->events[] = array("type" => "ERROR", "message" => "Missing required columns.");

      foreach ($importSpecDiff as $missing) {
        $this->events[] = array("type" => "ERROR", "message" => "Column header \"$missing\" missing.");
      }
      return false;
    }
  }

  private function initDBConnection()
  {
    global $global;
    $this->db = $global["db"];
  }

}

?>
