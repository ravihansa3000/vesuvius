/**
 * @name         PL User Services
 * @version      25
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0529
 */

// Google Maps junx

var map = new google.maps.Map(document.getElementById('mapCanvas'), {
	zoom: 2,
	center: new google.maps.LatLng(0,0),
	mapTypeId: google.maps.MapTypeId.TERRAIN
});



