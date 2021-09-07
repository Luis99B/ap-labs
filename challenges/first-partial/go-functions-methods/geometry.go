// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// X coordinate
func (p Point) X() float64 {
	return p.x
}

// Y coordinate
func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// generate a random point from range
func generatePoint() Point {
	var point Point
	min, max := -100, 100
	rand.Seed(time.Now().UnixNano())
	point.x = math.Round((rand.Float64()*float64(rand.Intn(max-min)+min))*100) / 100
	point.y = math.Round((rand.Float64()*float64(rand.Intn(max-min)+min))*100) / 100
	return point
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	sum += path[0].Distance(path[len(path)-1])
	return sum
}

// intersection validation
func onSegment(p, q, r Point) bool {
	if q.X() <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) && q.Y() <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y()) {
		return true
	}
	return false
}

// find the orientation of the ordered triplet
func orientation(p, q, r Point) int {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 { // colinear
		return 0
	} else if val > 0 { // clock wise
		return 1
	} else { // counterclock wise
		return 2
	}
}

// check if the points intersect
func doIntersect(p1, q1, p2, q2 Point) bool {
	// orientation of the 4 points
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)
	if o1 != o2 && o3 != o4 { // general case
		return true
	}
	// special cases
	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}
	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}
	return false
}

// generate the figure with valid points
func generateFigure(sides int) Path {
	fig := make(Path, sides)
	// generate random points
	for i := 0; i < sides; i++ {
		fig[i] = generatePoint()
	}
	// check intersections
	for i := 3; i < sides; i++ {
		intersect := doIntersect(fig[i-3], fig[i-2], fig[i-1], fig[i])
		if intersect {
			fig[i] = generatePoint()
			i--
		}
	}
	return fig
}

func main() {
	args := len(os.Args[1:])
	if args != 1 {
		fmt.Printf("ERROR: Illegal number of arguments, expected 1 argument\n")
		os.Exit(-1)
	}
	sides, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Printf("ERROR: %s is not an int\n", os.Args[1])
		os.Exit(-1)
	}
	if sides < 3 {
		fmt.Printf("ERROR: A figure must have 3 or more sides\n")
		os.Exit(-1)
	}
	fmt.Printf("- Generating a [%d] sides figure\n", sides)
	figure := generateFigure(sides)
	fmt.Printf("- Figure's vertices\n")
	for i := 0; i < sides; i++ {
		fmt.Printf("  - %.2f\n", figure[i])
	}
	fmt.Printf("- Figure's perimeter\n  - ")
	for i := 0; i < sides-1; i++ {
		fmt.Printf("%.2f + ", Distance(figure[i], figure[(i+1)%sides]))
	}
	fmt.Printf("%.2f ", Distance(figure[sides-1], figure[(sides)%sides]))
	perimeter := figure.Distance()
	fmt.Printf("= %.2f\n", perimeter)
}

//!-path
