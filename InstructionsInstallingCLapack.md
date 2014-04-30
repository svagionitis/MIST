Instructions Installing CLapack From Sources
--------------------------------------------
1.	Download the source code of CLapack with the following command

	`wget http://www.netlib.org/clapack/clapack.tgz`

2.	Unzip the compressed file with the following command

	`tar xvf clapack.tgz`

	A directory with the name of CLAPACK-3.2.1 will be created

3.	Go to this created directory

	`cd CLAPACK-3.2.1/`

4.	Copy the example `make.inc` to a file without the example extention, like the following

	`cp make.inc.example make.inc`

5.	Start building the libraries with the following command

	`make lib`
	`make lapacklib`

6.	The libraries that were created can be found with the following command:

		user@host:path$ find . -name \*.a
		./blas_LINUX.a
		./SRC/VARIANTS/LIB/choltop.a
		./SRC/VARIANTS/LIB/cholrl.a
		./SRC/VARIANTS/LIB/qrll.a
		./SRC/VARIANTS/LIB/lurec.a
		./SRC/VARIANTS/LIB/lull.a
		./SRC/VARIANTS/LIB/lucr.a
		./lapack_LINUX.a
		./tmglib_LINUX.a
		./F2CLIBS/libf2c.a

	We only need `blas_LINUX.a`, `lapack_LINUX.a` and `F2CLIBS/libf2c.a`. We copy these libraries to the lib directory in MIST (MIST/lib) and rename them like following

		cp blas_LINUX.a /path/to/MIST/lib/libblas.a
		cp lapack_LINUX.a /path/to/MIST/lib/liblapack.a
		cp F2CLIBS/libf2c.a /path/to/MIST/lib/

