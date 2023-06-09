
ALTER TABLE train_reports ADD CONSTRAINT obj_id_with_departure UNIQUE (original_scheduled_departure, amtrak_object_id) ;

ALTER TABLE stations_trains ADD CONSTRAINT station_and_train UNIQUE (station_id, train_id);

/*
CREATE INDEX IF NOT EXISTS idx_origSchDep_objectID
ON train_reports (original_scheduled_departure, amtrak_object_id);
*/

CREATE TABLE IF NOT EXISTS db_state (
    update_id integer primary key generated by default as identity,
    update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

ALTER TABLE stations_trains ADD COLUMN IF NOT EXISTS update_id INTEGER REFERENCES db_state;
ALTER TABLE train_reports ADD COLUMN IF NOT EXISTS update_id INTEGER REFERENCES db_state;
