amtraker = require('./amtraker_api.js');
db = require('../db/setup.js');
axios = require('axios');
client = db.createClient();

const wait = 
  (msec) => new Promise((resolve, _) => {
  setTimeout(resolve, msec);
});

all = async () => {
  // updates db to include new 'preferred' column on stations

  // gets list of bad stations with dupe station names
  // sorry about this, I prototyped in the console
  station_data = await client.query('select station_id from stations where display_name is null;');

  for (var x = 0; x < station_data.rows.length; x++ ) {
    station = station_data.rows[x];
    // try the API
    var station_info = (await amtraker.fetchOneStation(station.station_id));
    if ( station_info.length != 0 ) {
      station_info = station_info[station.station_id]
      console.log('updating');
      is_bus_stop = false;
      station_descriptor = '';
      name = "";
      display_name = `${station_info.name}, ${station_info.state}`;
      latitude = station_info.lat;
      longitude = station_info.lon;
      city = station_info.city;
      station_name = display_name;
      state_code = station_info.state;
      await client.query("INSERT INTO stations (station_id, name, station_name, display_name, city, latitude, longitude, is_bus_stop, station_descriptor, state_code) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10) ON CONFLICT (station_id) DO UPDATE SET name =$2,station_name =$3,display_name =$4,city = $5,latitude = $6,longitude = $7,is_bus_stop = $8,station_descriptor = $9, state_code=$10;", [station.station_id, name, display_name, display_name, city, latitude, longitude, is_bus_stop, station_descriptor, state_code]);
      name = await client.query('select display_name, city, state_code from stations where station_id=$1',[station.station_id]);
      details = name.rows[0];
      console.log(`Updated ${details.display_name} in ${details.city}, ${details.state_code}`);
    } else {
      await client.query(`DELETE FROM stations WHERE station_id=$1;`, [station.station_id]);
      console.log('removed station.station_id')
    }
    await wait(500);
  }
  return;
}

client.query("UPDATE stations SET display_name=$1, station_name=$1 WHERE station_id='CRH'", ['Cherry Hill, NJ']);

all();
