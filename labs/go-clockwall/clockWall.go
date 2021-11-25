package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
)

func handleConn(address string) {
	tzPort := strings.Split(address, "=")
	conn, err := net.Dial("tcp", tzPort[1])
	if err != nil {
		log.Print(err) // tcp error
	}

	done := make(chan int)

	go func() {
		s := bufio.NewScanner(conn)
		for s.Scan() {
			//os.Setenv("TZ", tzPort[0])
			fmt.Println(tzPort[0]+"\t:", s.Text())
		}
		done <- 2
	}()

	close(done)
}

func main() {
	args := len(os.Args[1:])
	if args < 1 {
		log.Fatal("ERROR: Not enough arguments")
	}
	done := make(chan int)
	for _, port := range os.Args[1:] {
		if len(strings.Split(port, "=")) != 2 {
			log.Fatal("ERROR: Address given is incorrect, must be <city>=localhost:<port>")
		}
		go handleConn(port)
	}
	for range os.Args[1:] {
		<-done
	}
}
