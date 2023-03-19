import '../css/search.css'
import {useContext } from 'react';
import {GlobalContext } from '../context/GlobalContext.js';
import {SearchContext } from '../context/Contexts.js';
import {bestTime} from '../context/setup.js';
import ResultRow from './ResultRow.js'
import selectTrain from './selectTrain.js'


function TrainDetails({train_details }) {
  const [searchTags, ] = useContext(SearchContext );
  const timeTags = [];
  if (typeof train_details !== 'undefined' ) {
    let stationIdsTags = searchTags.map( x => x.station_id);
    let matches = train_details.filter( x => stationIdsTags.includes(x.station_id));
    let today = new Date();
    for (var x = 0; x < matches.length; x++) {
      let station_details = matches[x];
      let time = new Date(bestTime(station_details));
      let day_diff = time.getDay() - today.getDay();
      let day_diff_string = 
        day_diff === 0 ? '' : (day_diff < 0 ? ' (' + day_diff.toString() + ')': ' (+' + day_diff.toString() + ')');
      timeTags.push( 
        <div class='train-time-box'>{station_details.station_id} {time.getHours()}:{time.getMinutes().toString().padStart(2,'0')}{day_diff_string}</div>
      );
    }
  }
  return (
    <div class="time-tags">
      {timeTags}
    </div>
  );
}

function SearchResultRow ( {train} ) {
  const [appState, setAppState ] = useContext(GlobalContext );
  const selectTrainFn = selectTrain(train.train_id, appState, setAppState);
  const label = `${train.first_station_name}, ${train.first_station_state_code} to ${train.last_station_name}, ${train.last_station_state_code}`;
  const details = (train.details && (<TrainDetails train_details={train.details} />)) || null;
  return (
    <ResultRow onClickPassThrough={selectTrainFn} resultLabel={label} resultDetails={details} />
  );
}

export default SearchResultRow;
