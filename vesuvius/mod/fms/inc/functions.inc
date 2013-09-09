<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * create a select field based on field options
 *
 * @param mixed $opt_field
 * @param mixed $label
 * @param string $select_opts
 * @param mixed $extra_opts
 * @access public
 * @return void
 */
function fms_form_opt_select($opt_field, $select_opts = "", $extra_opts = null)
{
  global $global;
  global $shn_tabindex;

  $sort = isset($extra_opts['sort']) ? $extra_opts['sort'] : null;
  $name = isset($extra_opts['name']) ? $extra_opts['name'] : $opt_field;
  $value = isset($_POST[$opt_field]) ? $_POST[$opt_field] : $extra_opts['value'];

  // Echo the select and label tags
  echo "<select name=\"" . $name . "\" " . $select_opts . " tabindex=\"" . ++$shn_tabindex . "\">\n";


  if ($sort == null) {
    $q = "select option_code, option_description from field_options where field_name='{$opt_field}'";
  } else {
    $q = "select option_code, option_description from field_options where field_name='{$opt_field}' order by $sort ";
  }

  try {
    $res = $global['db']->Execute($q);
  } catch (exception $e) {
    add_error($e->getMessage());
  }

  if (isset($extra_opts['select']) && $extra_opts['select'] == true) {
    echo "<option value='select'>- Select -</option>";
  }
  while (!$res == NULL && !$res->EOF) {
    $sel = ($res->fields['option_code'] == $value) ? 'selected="selected"' : null;
    echo "<option value=\"" . $res->fields['option_code'] . "\" " . $sel . ">" . _lc($res->fields['option_description']) . "</option> ";
    $res->MoveNext();
  }
  if (isset($extra_opts['all']) && $extra_opts['all'] == true) {
    $sel = (strtolower($value) == 'all') ? "selected=\"selected\"" : "";
    echo "<option value='all' $sel>[ALL]</option>";
  }
  echo "</select>";
}

/**
 * HTML dropdown list element
 *
 * This one does not create lable fields
 *
 * @param mixed $options
 * @param mixed $label
 * @param mixed $name
 * @param string $select_opts
 * @access public
 * @return void
 */
function fms_form_select($options, $name, $select_opts = "")
{
  global $global;
  global $shn_tabindex;

  $value = isset($_POST[$name]) ? $_POST[$name] : (isset($extra_opts['value']) ? $extra_opts['value'] : "");

  //to prevent warnings set $options to array() if it is null
  $options = ($options == null) ? array() : $options;

  echo "<select name=\"" . $name . "\" id=\"" . $name . "\" " . $select_opts . " tabindex=\"" . ++$shn_tabindex . "\">";

  foreach ($options as $opt_value => $desc) {
    $sel = ( $opt_value == $value ) ? 'selected="selected"' : null;
    echo "
			<option value=\"" . $opt_value . "\" " . $sel . ">" . $desc . "</option>
		";
  }
  echo "</select>";
}

function parsePhone($phone)
{

  $phone_number = array('areacode' => '', 'prefix' => '', 'suffix' => '');
  $phone_expr = '/\(?(\d{3})\W?\s?(\d{3})\W?(\d{4})/';

  preg_match_all($phone_expr, $phone, $matches);

  if (isset($matches[0][0])) {
    $phone_number["areacode"] = $matches[1][0];
    $phone_number["prefix"] = $matches[2][0];
    $phone_number["suffix"] = $matches[3][0];
  }

  return $phone_number;
}

function formatPhone($phone) {
  
  $p = parsePhone($phone);
  
  $format = "(%d) %d - %d";
  return sprintf($format, $p['areacode'], $p['prefix'], $p['suffix'] );
}


?>
