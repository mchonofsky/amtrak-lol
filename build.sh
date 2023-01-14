echo "running setup script"
if [ -z ${HEROKU_POSTGRESQL_AQUA_URL+x} ]; then
    echo 'USING LOCAL DB'
    export DATABASE_URL='postgres://mchonofsky:mchonofsky@localhost:5432/amtrak-lol'
else
    echo 'HEROKU AQUA DEFINED'
    export DATABASE_URL=$HEROKU_POSTGRESQL_AQUA_URL
fi
echo "database url is" $DATABASE_URL
echo 'Executing Database build'

############# DATABASE VERSIONING CONTROLLED BY TABLE versions
#
#
#

version_array=$( psql $DATABASE_URL -c 'copy (select migration_file from versions order by version_id) TO STDOUT;');

for item in $(ls db/*.sql | sort) ; do
    execute_file='true'
    for migration in $version_array ; do
        if [ $migration = $item ]; then
            execute_file='false'
        fi
    done
    if [ $execute_file = 'true' ]; then
        echo "Migrating database: executing $item"
        psql $DATABASE_URL -f $item && psql -a amtrak-lol -c "insert into versions ( migration_file) values ('${item}') ;"
    fi
done

