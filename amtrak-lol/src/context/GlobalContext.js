import { createContext } from 'react';

export const GlobalContext = createContext();

export default class AppStateClass {
  constructor() {
    this.currentDisplay = 'location-prompt';
    this.geolocation = undefined;
    this.selectedTrainDetails = undefined;
    // just destination, first station, trainId
    this.trains = [];
  }
}

export function getCurrentTrain(appState) {
  return appState.selectedTrainDetails;
}
