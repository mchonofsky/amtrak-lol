const fs = require('fs');
const axios = require('axios');
const https = require('https');
const amtrak = require('./amtrak')
let address = "maps.amtrak.com";
let path = "/services/MapDataService/trains/getTrainsData";
let url = new URL(`https://${address}${path}`);


amtrak.fetchTrainData().then( (data) => {
  fs.writeFile('data.json', JSON.stringify(data), (err) => {
    if (err) {
      throw err;
    }
    console.log('The data was successfully written to the file.');
  });
});

/*
axios.get(`https://${address}${path}`, {
  headers: {
    accept: '* /*', //REMOVE THE SPACE BETWEEN THE FIRST * and /*
    'user-agent': 'curl/7.79.1',
  },
  timeout: 5000, // it has a habit of timing out
}).then(
  (data) => {console.log(data);}
)
.catch((err) => {console.log(err);});

*/

/*
let apiCall = new Promise(function (resolve, reject) {
    var data = '';
    https.get(url, {}, res => {
        res.on('data', function (chunk){ data += chunk }) 
        res.on('end', function () {
           resolve(data);
        })
    }).on('error', function (e) {
      reject(e);
    });
});

apiCall.then((data) => {console.log('LEN IS', data.length);}).catch((err) => console.log(err));
*/

/*
try {
    let result = await apiCall;
} catch (e) {
    console.error(e);
} finally {
    console.log("LEN", result.length);
 // console.log('We do cleanup here');
}*/
/*
const axios_1 = require("axios");
const dataUrl = "https://maps.amtrak.com/services/MapDataService/trains/getTrainsData";
console.log("getting");
axios_1.get(dataUrl).then(
  (data) => {console.log(data);}
)
.catch((err) => {console.log(err);});
*/
