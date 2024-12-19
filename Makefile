all: build compile

build:
	docker build . -t map-generation

compile:
	image=$$(docker create map-generation) ; \
	docker cp $$image:/root/src/sfml-app ./ ; \
	docker rm -v $$image

clean:
	rm sfml-app
	docker image rm map-generation
