echo "running setup script"
psql $DATABASE_URL -f db/00_create_tables.sql
psql $DATABASE_URL -f db/01_create_index_train_reports.sql
psql $DATABASE_URL -f db/02_enter_stations.sql
psql $DATABASE_URL -f db/04_add_all_stations.sql
psql $DATABASE_URL -f db/05_add_more_stations_from_json.sql
psql $DATABASE_URL -f db/06_add_additional_stations.sql
