const axios = require('axios').default;

API_LOCATION = 'https://api-v3.amtraker.com/v3'

exports.fetchAllTrains = function fetchAllTrains() {
  return axios.get(`${API_LOCATION}/trains`)
    .then(function (response) {
      // handle success
      console.log('Got API response');
      return response.data;
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    })
};

exports.fetchAllStations = function fetchAllStations() {
  return axios.get(`${API_LOCATION}/stations`)
    .then(function (response) {
      // handle success
      console.log('Got API response for stations');
      return response.data;
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    })
};

exports.fetchOneStation = function fetchOneStation(station) {
  return axios.get(`${API_LOCATION}/stations/${station}`)
    .then(function (response) {
      // handle success
      console.log(`Got API response for station, here is ${station}`);
      return response.data;
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    })
};


exports.fetchOneTrain = function fetchOneTrain(train) {
  return axios.get(`${API_LOCATION}/trains/${train}`)
    .then(function (response) {
      // handle success
      console.log(`Got API response for station ${station}`);
      return response.data;
    })
    .catch(function (error) {
      // handle error
      console.log(error);
    })
};
