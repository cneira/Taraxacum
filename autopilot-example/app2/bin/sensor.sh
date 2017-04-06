# Cumulative number of dropped connections
unhandled() {
    local scraped=$(curl -s localhost:9032/solutions/app1)
    local accepts=$(echo ${scraped} | awk 'FNR == 3 {print $1}')
    local handled=$(echo ${scraped} | awk 'FNR == 3 {print $2}')
    echo $(expr ${accepts} - ${handled})
}

# ratio of connections-in-use to available workers
connections_load() {
    local scraped=$(curl -s localhost:9032/solutions/app1)
    local active=$(echo ${scraped} | awk '/Active connections/{print $3}')
    local waiting=$(echo ${scraped} | awk '/Reading/{print $6}')
    local workers=$(echo $(cat /etc/nginx/nginx.conf | perl -n -e'/worker_connections *(\d+)/ && print $1')
)
    echo $(echo "scale=4; (${active} - ${waiting}) / ${workers}" | bc)
}

clients_connected () {
	echo $(netstat -natl | grep :9032  | wc -l)
}
