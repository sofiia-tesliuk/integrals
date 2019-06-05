# Integral 
### Ackley's function

![image](https://drive.google.com/uc?export=view&id=1wH_40U0GQGJweO-6qOjPmPFqcT4reM0Q)

## Processing

### **Config file** <br /> 
Config file should have one attribute per line in such order, as they defined here (you can find example in `config.txt`): <br />
```
0.05        # absolute error
0.001       # relative error
4   	    # number of threads
20          # a
0.2         # b
6.2831853   # c
-100        # x1
100         # x2
-100        # y1
100         # y2
100         # initial steps
100000      # max steps
```


### **Compilation** <br />
Creating new working directory: <br /> 
`mkdir build` <br />

Go to working directory: <br /> 
`cd build` <br />

Compiling cpp files: <br /> 
`cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..` <br /> 
`make` <br />


### Running programs
Run consistent program: <br />
`./consistent <config_filename>` <br />

Run parallel program: <br /> 
`./parallel <config_filename>` <br />

If you do not provide `<config_filename>`, program will be looking for `config.txt` in local directory.

### Running test program
Go to upper directory: <br />
`cd ..` <br />

Run test: <br />
`python3 test_and_compare.py <number_of_tests>` <br /> 

If you do not provide `<number_of_tests>`, program will will set `number_of_tests = 10000`.

## Results
For such configuration: <br />
```
0.05        # absolute error
0.001       # relative error
20          # a
0.2         # b
6.2831853   # c
-100        # x1
100         # x2
-100        # y1
100         # y2
100         # initial steps
100000      # max steps
```
Number of tests: 10000

**Consistent:** **2816** microseconds

**Parallel:** <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**1** threads: **2912** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**2** threads: **702** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**3** threads: **64** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**4** threads: **68** microseconds <br/>
![image](https://drive.google.com/uc?export=view&id=1bVUEsYrfoJmIMa5cCo7_zqVVAph7y3Wm) <br />
![#2b6d36](https://placehold.it/15/2b6d36/000000?text=+) consistent <br />
![#4b61a3](https://placehold.it/15/4b61a3/000000?text=+) parallel

<br/>
<br/>

**Parallel:** <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**10** threads: **7887** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**100** threads: **313** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**1000** threads: **3564** microseconds <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**10000** threads: **36472** microseconds <br/>
![image](https://drive.google.com/uc?export=view&id=11-krH2r-O0ibH0jLXji4eB5RqLpItUBA) <br />
![#2b6d36](https://placehold.it/15/2b6d36/000000?text=+) consistent <br />
![#4b61a3](https://placehold.it/15/4b61a3/000000?text=+) parallel
