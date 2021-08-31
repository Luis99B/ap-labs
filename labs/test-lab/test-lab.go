package main

import (
	"fmt"
	"os"
)

func main() {
	n := len(os.Args[1:])
	if n < 1 {
		fmt.Printf("ERROR, no name given\n")
	} else {
		fmt.Printf("Hello")
		for i := 0; i < n; i++ {
			fmt.Printf(" %s", os.Args[i+1])
		}
		fmt.Printf("\n")
	}
}
