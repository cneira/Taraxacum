#  C++ Microservices in Containers with ContainerPilot

Example microservices implementation using Taraxacum  and Docker. Below is an architectural diagram depicting the composition of services that make up the project. When everything is working a frontend web application is accessible that will display a set of graphs using sensor data.


## Usage

### Development

In local development you can start the microservices by running

```sh
$ docker-compose -f local-compose.yml up -d
```

Then curl localhost/app1 and you'll get a response composed of strings from app1 and app2

### Production

When deploying to Triton, first setup your environment then run docker-compose. Below is an example of setting your environment variables then pushing the code to production.

```sh
$ ./setup.sh
$ eval $(triton env)
$ docker-compose up -d
```

## Credits
I used this project as template
* https://github.com/autopilotpattern/nodejs-example
