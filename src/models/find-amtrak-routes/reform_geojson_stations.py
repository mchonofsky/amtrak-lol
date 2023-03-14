import json

gj = json.load(open('Amtrak_Stations.geojson'))
stations = gj['features']

f = open('db/12_add_station_columns.sql','w')

f.write('''
ALTER TABLE stations ADD COLUMN IF NOT EXISTS is_bus_stop BOOLEAN;                                   
ALTER TABLE stations ADD COLUMN IF NOT EXISTS city VARCHAR(64);                                      
ALTER TABLE stations ADD COLUMN IF NOT EXISTS display_name VARCHAR(128);                             
-- multiple 'station_name' with the same 'name'                                                      
ALTER TABLE stations ADD COLUMN IF NOT EXISTS station_name VARCHAR(128);                             
ALTER TABLE stations ADD COLUMN IF NOT EXISTS station_descriptor VARCHAR(128);
''')

r = lambda x: '\'' + x.replace('\'','\'\'') + '\''
f.write('\nBEGIN;\n\n')
for station_exp in stations:
    station = station_exp['properties']
    sub_name = station.get('Name','').strip()
    station_name = station.get('StationNam','').strip()
    city = station.get('City', '')
    code = station.get('Code', '')
    coordinates = station_exp['geometry']['coordinates']

    name_to_use = (sub_name and (sub_name if city in sub_name else station_name + ' - ' + sub_name) ) or station_name
    is_bus_stop = 'true' if station.get('StnType', 'TRAIN') == 'BUS' else 'false'
    lat = coordinates[1]
    lon = coordinates[0]
    station_descriptor = station.get('StaType','')
    
    code = r(code)
    sub_name = r(sub_name)
    station_name = r(station_name)
    city = r(city)
    station_descriptor = r(station_descriptor)
    name_to_use = r(name_to_use)

    sql_string = (
f'''INSERT INTO stations (station_id, name, station_name, display_name, city, latitude, longitude, is_bus_stop, station_descriptor) 
VALUES ({code}, {sub_name}, {station_name}, {name_to_use}, {city}, {lat}, {lon}, {is_bus_stop}, {station_descriptor}) ON CONFLICT (station_id) DO UPDATE SET
 name = {sub_name},
station_name = {station_name},
display_name = {name_to_use},
city = {city},
latitude = {lat},
longitude = {lon},
is_bus_stop = {is_bus_stop},
station_descriptor = {station_descriptor};
'''
).replace('\n','') + '\n'
    
    f.write(sql_string)

f.write('\nCOMMIT;')
