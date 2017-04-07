clients_connected () {
	echo $(netstat -natl | grep :8040  | wc -l)
}
