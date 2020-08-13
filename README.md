# vietnamese-lunar-calendar
A program to convert the current solar date into Vietnamese lunar date (TOTALLY DIFFERENT FROM CHINESE LUNAR DATE)

This project is based off [Ho Ngoc Duc](http://www.informatik.uni-leipzig.de/~duc/)'s Java version, written in C to ensure the best performance. 

You can read on how to convert solar date to Vietnamese lunar date [here.](http://www.informatik.uni-leipzig.de/~duc/amlich/calrules_en.html)

## Performance comparison between the Java and the C version
```
$ gcc -o -lm VietCalendar VietCalendar.c
$ time ./VietCalendar
2020-06-24
________________________________________________________
Executed in    6.86 millis    fish           external
   usr time    1.10 millis  236.00 micros    0.87 millis
   sys time    5.36 millis  1070.00 micros    4.29 millis
   
$ time java VietCalendar.java
2020-06-24
________________________________________________________
Executed in  815.77 millis    fish           external
   usr time  1594.94 millis  334.00 micros  1594.61 millis
   sys time   94.90 millis  705.00 micros   94.20 millis
   
$ javac VietCalendar.java
$ time java VietCalendar
2020-06-24
________________________________________________________
Executed in  230.86 millis    fish           external
   usr time  305.43 millis    0.00 micros  305.43 millis
   sys time   40.88 millis  793.00 micros   40.09 millis
```

# TODO
- [ ] Add the ability to format the output string, like `date`

- [ ] Add the ability to convert any day, not just today
