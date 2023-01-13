ALTER TABLE stations_trains ADD CONSTRAINT unique_station_with_train UNIQUE (station_id, train_id);
ALTER TABLE train_reports DROP CONSTRAINT obj_id_with_departure;
ALTER TABLE train_reports ADD CONSTRAINT obj_id_only UNIQUE (amtrak_object_id) ;
