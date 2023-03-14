import axios from 'axios';

export function sanitizeTime(timeString) {
  try {
    return new Date(Date.parse(timeString));
  }
  catch ( e ) {
    console.log('ERROR: timeString is', timeString);
    throw e;
  }
};

export function bestTime(station_record) {
  return Date.parse(station_record.scheduled_arrival || station_record.scheduled_departure);
  
  // old method - falls down the list to enable accurate sorting of rows
  /*
  return [
    station_record.scheduled_arrival,
    station_record.scheduled_departure,
    station_record.posted_arrival,
    station_record.posted_departure
  ].reduce( (time, option) => time || option )
  */
};

export function ascendingSort(a,b) { 
  return a > b ? 1 : (a < b ? -1 : 0) };

export function stDistance(lat1, lon1, lat2, lon2) {
  var R = 6371; // Radius of the earth in km
  var dLat = deg2rad(lat2-lat1);  // deg2rad below
  var dLon = deg2rad(lon2-lon1);
  var a =
    Math.sin(dLat/2) * Math.sin(dLat/2) +
    Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) *
    Math.sin(dLon/2) * Math.sin(dLon/2)
    ;
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
  var d = R * c; // Distance in km
  return d;
}

function deg2rad(deg) {
  return deg * (Math.PI/180)
}

async function getStations() {
  console.log('getting stations');
  let stations = (await axios.post('/api/get_stations')).data;
  // comes as name, state_code, station_id
  //
  //// three characterists: search display, search target, station code
  
  const stations_to_display = stations.map(station_entry => {
    return {
      search_display: station_entry.display_name,
      search_target: station_entry.display_name.toLowerCase(),
      station_id: station_entry.station_id
    }
  });
  
  const stations_to_display_entry_index = stations.map(station_entry => {
    return {
      search_display: station_entry.display_name,
      search_target: station_entry.station_id.toLowerCase(),
      station_id: station_entry.station_id
    }
  });
  
  const stations_to_display_city = stations.map(station_entry => {
    return {
      search_display: station_entry.display_name,
      search_target: station_entry.city.toLowerCase(),
      station_id: station_entry.station_id
    }
  });
  return stations_to_display_city.concat(stations_to_display).concat(stations_to_display_entry_index);
}

export function makeSwitchTo(display, appState, setAppState) {
  return () => switchTo(display, appState, setAppState);
}

export async function switchTo (display, appState, setAppState) {
  console.log('display is', display);
  appState.currentDisplay = display;
  // this makes it refresh furiously, oops
  /*
  const titleData = {
    'train-search': 'Search for trains',
    'location-prompt': 'Amtrak.lol',
    'multiple-trains': 'Select your train',
    'time-display': 'Your train'
  }
  */
  //window.history.pushState({'page': display}, titleData[display] || 'Amtrak.lol', `${window.location.origin}?${display}`);
  if ( display === 'train-search' && (! appState.searchStations )) {
      console.log('set', await getStations());
      setAppState({...appState, searchStations: await getStations(), currentDisplay: display});
      console.log('goti');
  } else {
    setAppState({...appState, currentDisplay:display});
  };
}
