function initDate() {
    $("#checkInDate").datepicker({ dateFormat: 'yy-mm-dd' });
    $("#checkOutDate").datepicker({ dateFormat: 'yy-mm-dd' });
    $("#dob").datepicker({ dateFormat: 'yy-mm-dd' });
}

function get_search_query() {
    $.tgrid = {};
    var r = new Object();
    r.name_query = $("#searchbox").val();
    r.orgId     = $("#orgId").val();
    r.volId     = $("#volId").val();
    r.facility_group = $("#facility_group").val();
    r.facility = $("#facility").val();
    r.limit = $("#maxlimit").val();
    r.page = $("#pagenum").val();
    r.sidx = $.tgrid.sidx = 'p_uuid';
    r.sord = $.tgrid.sord = 'asc';
    var rj = array2json(r)
    return rj;
}

// converts array to json for export >> from: http://goo.gl/ZVNnr
function array2json(arr) {
    var parts = [];
    var is_list = (Object.prototype.toString.apply(arr) === '[object Array]');

    for(var key in arr) {
        var value = arr[key];
        if(typeof value == "object") { //Custom handling for arrays
            if(is_list) {
                parts.push(array2json(value)); // :RECURSION:
            } else {
                parts[key] = array2json(value); // :RECURSION:
            }
        } else {
            var str = "";
            if(!is_list) {
                str = '"' + key + '":';
            }
            // Custom handling for multiple data types
            if(typeof value == "number") {
                str += value; //Numbers
            } else if(value === false) {
                str += 'false'; //The booleans
            } else if(value === true) {
                str += 'true';
            } else {
                str += '"' + value + '"'; //All other things
            }
            // todo: Is there any more datatype we should be in the lookout for? (Functions?)
            parts.push(str);
        }
    }
    var json = parts.join(",");
    if(is_list) {
        return '[' + json + ']';//Return numerical JSON
    }
    return '{' + json + '}';//Return associative JSON
}
