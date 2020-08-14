/*
 * Implementation of VietCalendar.java (author Ho Ngoc Duc) in C
 * Author: beerpsi
 * https://github.com/beerpsi
 * Ho Ngoc Duc's website: http://www.informatik.uni-leipzig.de/~duc/
 */

#include <stdio.h> /* printing out the time */
#include <math.h> /* sin, M_PI */
#include <time.h> /* getting system time to convert */

struct date {
    char date_mday; /* Day of the month */
    char date_mon; /* Month: 1-12 */
    long int date_year; /* Just a year number */
    char date_leap; /*indicate leap*/    
};

/* Return the number of days since 1/1/-4713 (julian calendar)*/
long long int jd_from_date(char dd, char mm, long int yyyy)
{
    char a = (14 - mm) / 12;
    long long int y = yyyy + 4800 - a;
    int m = mm + 12*a - 3;
    long long int jd = dd + (153*m+2)/5 + 365*y + y/4 - y/100 + y/400 - 32045;
    if (jd < 2299161) {
        jd = dd + (153*m+2)/5 + 365*y + y/4 - 32083;
    }
    return jd;
}

/* jd is the number of days sinice 1/1/-4713 */
struct date jd_to_date(int jd){
    int a, b, c;
    if (jd > 2299160) {
        a = jd + 32044;
        b = (4*a+3)/146097;
        c = a - (b*146097)/4;
    }
    else {
        b = 0;
        c = jd + 32082;
    }
    int d = (4*c+3)/1461;
    int e = c - (1461*d)/4;
    int m = (5*e+2)/153;
    struct date return_var;
    return_var.date_mday = (char)(e - (153*m+2)/5 + 1);
    return_var.date_mon  = (char)(m + 3 - 12*(m/10));
    return_var.date_year = (long)(b*100 + d - 4800 + m/10);
    return return_var;
}

double sun_longitude(double jdn){
    double T  = (jdn - 2451545.0) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
    double T2 = T*T;
    double dr = M_PI/180; //degree to radian
    double M = 357.52910 + 35999.05030*T - 0.0001559*T2 - 0.00000048*T*T2;// mean anomaly, degree
    double L0 = 280.46645 + 36000.76983*T + 0.0003032*T2; // mean longitude, degree
    double DL = (1.914600 - 0.004817*T - 0.000014*T2)*sin(dr*M) + (0.019993 - 0.000101*T)*sin(dr*2*M) + 0.000290*sin(dr*3*M);
    double L = L0 + DL; // true longitude, degree
    L = L - 360*((int)(floor(L/360))); // Normalize to (0,360)
    return L;
}

double new_moon(int k) {
    double T = k/1236.85; // Time in Julian centuries from 1900 January 0.5
    double T2 = T * T;
    double T3 = T2 * T;
    double dr = M_PI/180;
    double Jd1 = 2415020.75933 + 29.53058868*k + 0.0001178*T2 - 0.000000155*T3 + 0.00033*sin((166.56 + 132.87*T - 0.009173*T2)*dr); // Mean new moon
    double M = 359.2242 + 29.10535608*k - 0.0000333*T2 - 0.00000347*T3; //Sun's mean anomaly
    double Mpr = 306.0253 + 385.81691806*k + 0.0107306*T2 + 0.00001236*T3;// Moon's mean anomaly
    double F = 21.2964 + 390.67050646*k - 0.0016528*T2 - 0.00000239*T3; //Moon's argument of latitude
    double C1=(0.1734 - 0.000393*T)*sin(M*dr) + 0.0021*sin(2*dr*M) - 0.4068*sin(Mpr*dr) + 0.0161*sin(dr*2*Mpr) - 0.0004*sin(dr*3*Mpr) + 0.0104*sin(dr*2*F) - 0.0051*sin(dr*(M+Mpr)) - 0.0074*sin(dr*(M-Mpr)) + 0.0004*sin(dr*(2*F+M)) - 0.0004*sin(dr*(2*F-M)) - 0.0006*sin(dr*(2*F+Mpr)) + 0.0010*sin(dr*(2*F-Mpr)) + 0.0005*sin(dr*(2*Mpr+M));
    double deltat;
    if (T < -11){
        deltat= 0.001 + 0.000839*T + 0.0002261*T2 - 0.00000845*T3 - 0.000000081*T*T3;
    }
    else {
        deltat= -0.000278 + 0.000265*T + 0.000262*T2;
    }
    double Jd_new = Jd1 + C1 - deltat;
    return Jd_new;
}

double get_sun_longitude(int day_number, double time_zone) {
    return sun_longitude(day_number - 0.5 - time_zone/24);
}

int get_new_moon_day(int k, double time_zone){
    double jd = new_moon(k);
    return (int)(floor(jd + 0.5 + time_zone/24));
}

int get_lunar_month_11(int yyyy, double time_zone){
    double off = jd_from_date(31, 12, yyyy) - 2415021.076998695;
    int k = (int)(floor(off/29.530588853));
    int nm = get_new_moon_day(k, time_zone);
    int sun_long= (int)(floor(get_sun_longitude(nm, time_zone)/30));
    if (sun_long >= 9){
        nm = get_new_moon_day(k-1, time_zone);
    }
    return nm;
}

int get_leap_month_offset(int a11, double time_zone){
    int k = (int)(floor(0.5 + (a11 - 2415021.076998695) / 29.530588853));
    int last;
    int i = 1;
    int arc = (int)(floor(get_sun_longitude(get_new_moon_day(k+i, time_zone), time_zone)/30));
    do {
        last = arc;
        i++;
        arc = (int)(floor(get_sun_longitude(get_new_moon_day(k+i, time_zone), time_zone)/30));
    } while (arc != last && i < 14);
    return i-1;
}

struct date convert_solar_lunar(int dd, int mm, int yyyy, double time_zone){
    int lunar_day, lunar_month, lunar_year, lunar_leap;
    int day_number = jd_from_date(dd, mm, yyyy);
    int k = (int)(floor((day_number - 2415021.076998695) / 29.530588853));
    int month_start = get_new_moon_day(k+1, time_zone);
    if (month_start > day_number){
        month_start = get_new_moon_day(k, time_zone);
    }
    int a11 = get_lunar_month_11(yyyy, time_zone);
    int b11 = a11;
    if (a11 >= month_start){
        lunar_year=yyyy;
        a11 = get_lunar_month_11(yyyy-1, time_zone);
    }
    else {
        lunar_year = yyyy+1;
        b11 = get_lunar_month_11(yyyy+1, time_zone);
    }
    lunar_day = day_number-month_start+1;
    int diff = (int)(floor((month_start - a11)/29));
    lunar_leap = 0;
    lunar_month = diff + 11;
    if (b11 - a11 > 365) {
        int leap_month_diff = get_leap_month_offset(a11, time_zone);
        if (diff >= leap_month_diff){
            lunar_month = diff + 10;
            if (diff == leap_month_diff){
                lunar_leap = 1;
            }
        }
    }
    if (lunar_month > 12) {
        lunar_month = lunar_month - 12;
    }
    if (lunar_month >= 11 && diff < 4){
        lunar_year--;
    }
    struct date return_var = {(char)lunar_day, (char)lunar_month, (long)lunar_year, (char)lunar_leap};
    return return_var;
}

struct date convert_lunar_solar(int lunar_day, int lunar_month, int lunar_year, int lunar_leap, double time_zone){
    int a11, b11;
    if (lunar_month < 11){
        a11 = get_lunar_month_11(lunar_year-1, time_zone);
        b11 = get_lunar_month_11(lunar_year, time_zone);
    }
    else {
        a11 = get_lunar_month_11(lunar_year, time_zone);
        b11 = get_lunar_month_11(lunar_year+1, time_zone);
    }
    int k = (int)(floor(0.5 + (a11 - 2415021.076998695) / 29.530588853));
    int off = lunar_month - 11;
    int leap_month = 0;
    int leap_off = 0;
    if (off < 0){ off = off + 12; }
    if (b11 - a11 > 365){
        leap_off = get_leap_month_offset(a11, time_zone);
        leap_month = leap_off - 2;
    }
    if (leap_month < 0){ leap_month = leap_month + 12; }
    if (lunar_leap != 0 && lunar_month != leap_month){
        struct date return_var = {0, 0, 0};
        return return_var;
    }
    else if (lunar_leap != 0 || off >= leap_off){
        off++;
    }
    int month_start = get_new_moon_day(k+off, time_zone);
    return jd_to_date(month_start+lunar_day-1);
}

int main(){
    double time_zone = 7.0;
    time_t now = time(0);
    struct tm *now_tm = localtime(&now);
    char day = (char)now_tm->tm_mday;
    char month = (char)(now_tm->tm_mon+1); /* Add one so the month value is 1-12 instead of 0-11 */
    long year = (long)(now_tm->tm_year+1900); /* Add 1900 to get current year */
    struct date l = convert_solar_lunar(day, month, year, time_zone);
    printf("%ld-%02d-%02d",l.date_year,l.date_mon,l.date_mday);
}

    
    


