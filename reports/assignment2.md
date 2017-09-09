Question 1. What are 2 things that you learned from this assignment?

Answer:

The two most important things which I learnt from this assignment are as follows:
1. While implementing polling and message passing functions, I discovered how processes interact with each other. When I added a print statement in the while loop of the polling function before the polling operation, I saw that the process was staying for some time before it got context switched to its neighbouring process. 
2. I also learnt how to segregate code to achieve modularity because of which when, I had to change my message passing function a little in order to fix a bug, I did not have to distrupt my entire set up, which I thought was pretty cool.

Question 2. What are 2 mistakes that you made in this assignment, and how did you discover and fix these mistakes?

Answer:

Highlighting two mistakes:
1. When I completed coding polling functiona and started executing it, I realized that the parent function was not completing its execution at all. This was because my condition to check if the child processes were done executing was messed up. I was passing a variable count (global variable) to count the completion of the number of processes. Instead of passing the address of the variable to a pointer, I passed the variable as is and this caused the code to hang. When I started printing the address of the count variable in the parent and then in the child processes, I discovered that I, had made a mistake and fixed it.
2. While handling argument parsing step, I used a character array to declare the default implementation as "poll". Then I declared a character pointer and pointed it to the address of the character array. Later in the code, I stored the value given by the user into the pointer and tried to perform the appropriate implementation. My code failed as expected, I was able to find out through some internet surfing that, I can declare character array and simply pass the value of that array as is, to a character pointer, and just as easily assign a new character array value to that pointer. This helped me modify my code and get it to work as required.

Question 3: If you were restarting this assignment, what would you change about how you went about completing this assignment?

Answer:

If I had to do it over again, I probably would start with smaller chunks of code. For example, first implmenting arg parser perfectly and checking all the conditions and printing out error messages as deemed appropriate. Then, implement polling and integrate that code into the arg parser code and testing it for various input values and ensuring correctness of implementation. And building the messga passing/semaphores on top of that. 
