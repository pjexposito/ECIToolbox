function HTTPGET(url) {
	var req = new XMLHttpRequest();
	req.open("GET", url, false);
	req.send(null);
	return req.responseText;
}

var ObtenDatos = function() {
  var response = HTTPGET("https://dl.dropboxusercontent.com/u/119376/data.dat");
  //console.log(response);

	var json = JSON.parse(response);
	var data0 = json.main.data0;
	var data1 = json.main.data1;
  var data2 = json.main.data2;
  var data3 = json.main.data3;
  var data4 = json.main.data4;
	var data5 = json.main.data5;
	var data6 = json.main.data6;
  var data7 = json.main.data7;
  var data8 = json.main.data8;
  var data9 = json.main.data9;	
  var data10 = json.main.data10;
	var data11 = json.main.data11;
  var data12 = json.main.data12;
  var data13 = json.main.data13;
  var data14 = json.main.data14;
  var dict = {"0" : data0, "1": data1, "2": data2, "3": data3, "4": data4, "5": data5, "6": data6, "7": data7, "8": data8, "9": data9, "10": data10, "11": data11, "12": data12, "13": data13, "14": data14};
	
	Pebble.sendAppMessage(dict);
};

Pebble.addEventListener("ready",
  function(e) {
	//App is ready to receive JS messages
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	ObtenDatos();
  }
);