# vietnamese-lunar-calendar
Phần mềm chuyển đổi từ ngày dương lịch hôm nay sang ngày âm lịch hôm nay

Phần mềm này dựa trên bản Java của [Hồ Ngọc Đức](http://www.informatik.uni-leipzig.de/~duc/), được viết bằng ngôn ngữ C để đảm bảo về tốc độ

## So sánh về tốc độ giữa bản Java và bản C
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

## TODO
- [ ] Cho phép người dùng tự format đầu ra
- [ ] Cho phép người dùng chuyển đồi từ bất kì ngày nào

