import {switchTo} from '../context/setup.js'
import axios from 'axios';

function selectTrain(train_id, appState, setAppState) {
  /* returns a function that can be used onClick, below */
  return async () => {
    console.log(axios.post, axios)
    let selected_train_details = (await axios.post('/api/train_details', {train_id: train_id})).data;
    appState.selectedTrainDetails = selected_train_details;
    setAppState({...appState, selectedTrainDetails: selected_train_details});
    switchTo('time-display', appState, setAppState);
  }
}
export default selectTrain;
