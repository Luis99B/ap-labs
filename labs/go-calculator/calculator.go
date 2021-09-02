package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	res := 0
	if operator == 1 { // add
		for i := 0; i < len(values); i++ {
			res += values[i]
		}
	} else if operator == 2 { // sub
		res = values[0]
		for i := 1; i < len(values); i++ {
			res -= values[i]
		}
	} else if operator == 3 { // mult
		res = values[0]
		for i := 1; i < len(values); i++ {
			res *= values[i]
		}
	}
	return res
}

// parse the value to int and catch errors
func parse(x string) int {
	val, err := strconv.Atoi(x)
	if err != nil { // parse error
		log.Fatalf("ERROR: %s is not an int", x)
	}
	return val
}

func main() {
	args := len(os.Args[1:])
	if args < 3 {
		log.Fatal("ERROR: Not enough arguments")
	}
	op := os.Args[1]
	size := len(os.Args[2:])
	values := make([]int, size)
	i := 0
	for j := 2; i < size; j++ {
		values[i] = parse(os.Args[j])
		i++
	}
	if op == "add" { // operator is add
		res := calc(1, values)
		for i := 0; i < size-1; i++ {
			fmt.Printf("%d + ", values[i])
		}
		fmt.Printf("%d = %d\n", values[size-1], res)
	} else if op == "sub" { // operator is sub
		res := calc(2, values)
		for i := 0; i < size-1; i++ {
			fmt.Printf("%d - ", values[i])
		}
		fmt.Printf("%d = %d\n", values[size-1], res)
	} else if op == "mult" { // operator is mult
		res := calc(3, values)
		for i := 0; i < size-1; i++ {
			fmt.Printf("%d * ", values[i])
		}
		fmt.Printf("%d = %d\n", values[size-1], res)
	} else { // invalid operator
		log.Fatalf("ERROR: %s is an Invalid Operator\n", op)
	}
}
