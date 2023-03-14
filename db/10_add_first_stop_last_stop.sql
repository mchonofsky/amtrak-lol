
ALTER TABLE train_reports ADD COLUMN first_station  varchar(3) CONSTRAINT first_station_fk_station REFERENCES stations(station_id);
ALTER TABLE train_reports ADD COLUMN last_station varchar(3) CONSTRAINT last_station_fk_station REFERENCES stations(station_id)   ;
