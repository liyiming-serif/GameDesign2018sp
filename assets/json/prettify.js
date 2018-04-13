const fs = require("fs");

var filename = process.argv[2];
fs.readFile(filename, 'utf8', function (err, data){
	if(err){
		throw err;
	}
	jsonData = JSON.parse(data);
	output = JSON.stringify(jsonData, null, 4);
	fs.writeFile(filename, output, function(e){
        if(e){
            console.log(e);
        }
    });
});