var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
var url = require('url');
const { exec } = require("child_process");
var path = require('path');
var awsIot = require('aws-iot-device-sdk');

mongoose.connect('localhost:27017/videoDb');
var Schema = mongoose.Schema;

var videoData = new Schema({
  Title: String,
  URLstring: String,
  Description: String
}, {collection: 'videoInfos'});

var videoInfos = mongoose.model('videoInfo', videoData);

var humidity = 0;
var temperature = "0";

var device = awsIot.device({
  keyPath: "C:\\nodejs\\MiniProject\\certs\\df7b8df7ef-private.pem.key",
  certPath: "C:\\nodejs\\MiniProject\\certs\\df7b8df7ef-certificate.pem.crt",
  caPath: "C:\\nodejs\\MiniProject\\certs\\AmazonRootCA1.pem",
  clientId: "qwerty",
  host: "a3tpzmz206oll8-ats.iot.us-east-1.amazonaws.com"
});

device
  .on('connect', function() {
    console.log('connect');
    device.subscribe('AutoGarden');
    device.publish('AutoGarden', JSON.stringify({ test_data: 1}));
  });

device
  .on('message', function(topic, payload) {
    temperature = payload.toString();
    console.log('message', topic, payload.toString());
  });


router.get('/', function(req, res, next) {
  videoInfos.find()
      .then(function(doc) {
        console.log("Array length is: " + doc.length);
        for(var ii = 0; ii < doc.length; ii++){
          console.log(doc[ii].Title);
        }
        res.render('index', {title: "My First Page"});
      });
});

router.get('/getTemp', function(req, res, next) {
  res.render('getTemp', {temperature: "25"});
});


router.get('/getHumidity', function(req, res, next) {
  var q = url.parse(req.url, true);
  res.send("" + humidity++);
});

router.post('/login', function(req, res, next) {
  var myFormObject = {
    username: "",
    password: ""
  };
  myFormObject.username = req.body.username;
  myFormObject.password = req.body.passwd;

  res.render('loginResponse', {title: "Login status", yourUsername: myFormObject.username, yourPassword: myFormObject.password});
});

router.post('/jsonExample', function(req, res, next) {
  var rxObject = req.body;
  console.log("" + rxObject.myName);
  console.log("" + rxObject.myNum);
  var txObject = {
    number: 101010,
    color: ""
  };
  res.json(txObject);
});

router.post('/licencePlate', function(req, res, next) {
  var jsObject = req.body;
  console.log("" + jsObject.plateNumber);
});

router.post('/getLicencePlate', function(req, res, next) {
  var jsObject = {
    number: "",
    color: ""
  }
  jsObject.number = "17D12345";
  res.json(jsObject);
});

router.post('/runScript', function(req, res, next) {
  var scrObject = req.body;
  var stdOutString = "";
  console.log(scrObject.scriptInput);

  exec(scrObject.scriptInput, (error, stdout, stderr) => {
    if (error) {
        console.log(`error: ${error.message}`);
        return;
    }
    if (stderr) {
        console.log(`stderr: ${stderr}`);
        return;
    }
    stdOutString = `${stdout}`;
    console.log(stdOutString);
    scrObject.scriptOutput = stdOutString;
    res.json(scrObject);
  });
});

module.exports = router;
