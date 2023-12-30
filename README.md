# bananagrams-god

A tool to help you beat your family at bananagrams

__Example usage:__

```
>>> add letters askdjdeoorurdquioaytartbnmhjk
----------
     e   
     q   
     u   
   j i   
 s o t o 
 junkyard
 a a  d r
 m h  d  
 b       
rot      
 k       
----------
Remaining letters: 
>>> add letters artyubhgpi
----------
          eg  
          q   
          u   
  u     j i   
bypaths o t o 
      junkyard
    ria a  d r
      m h  d  
      b       
     rot      
      k       
----------
Remaining letters: 
>>> quit
```

## Info

This is something I made for fun a couple years ago while I was flying across the Atlantic. I was really bored on the plane and thought about how cool it would be to write a program that would play bananagrams for me. So, I utilized the painstaking nine-hour flight, and my subsequent connection flights, to start designing this program.

The way it works is you give it a list of letters, and it progressively builds up a viable bananagrams board from those letters. It is a greedy algorithm that puts words onto the board one at a time, getting rid of the "troublesome" letters first.

## Usage

First, use the CMake script I provided to build the project. Then run it with ```./bananagrams-god```.

Once the program is running, you can type ```help``` for info and ```quit``` to exit the program at any time.

Note: I discovered a bug today where using the ```remove``` command doesn't correctly keep track of remaining letters. Therefore, you should manually keep track of your remaining letters (you should be naturally doing this anyway, simply by virtue of the way the game is designed).

## Strategy/ Advice

I've noticed it works a lot better when it has a lot of letters to work with. Therefore, my recommendation is that whenever you run out of letters and get a "peel" you should "dump" your new letter, and maybe do 5 or 6 more "dumps" until you have at least 10 letters to work with. This makes the resulting board cleaner and less cluttered because the program is able to build longer words, and makes the game go by a lot faster.

## Disclaimer

The "wordlist.txt" has some questionable words in it that might not fly in a legit bananagrams game. It probably wouldn't hurt to look for a better wordlist to use as a source dictionary if you're interested in using this program seriously.
