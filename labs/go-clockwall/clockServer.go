// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strconv"
	"time"
)

func TimeIn(t time.Time, name string) (time.Time, error) {
	loc, err := time.LoadLocation(name)
	if err == nil {
		t = t.In(loc)
	}
	return t, err
}

func handleConn(c net.Conn) {
	defer c.Close()
	tz := os.Getenv("TZ")
	//fmt.Println("TZ: ", tz)
	for {
		t, errT := TimeIn(time.Now(), tz)
		if errT != nil {
			fmt.Println(tz, "<time unknown>")
		}
		_, err := io.WriteString(c, t.Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	args := len(os.Args[1:])
	if args < 2 {
		log.Fatal("ERROR: Not enough arguments")
	}
	portParam := os.Args[1]
	if portParam != "-port" {
		log.Fatal("ERROR: First parameter must be '-port'")
	}
	var port = flag.Int("port", 8000, "Port of the clock server")
	flag.Parse()
	host := "localhost:" + strconv.Itoa(*port)
	listener, err := net.Listen("tcp", host)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
