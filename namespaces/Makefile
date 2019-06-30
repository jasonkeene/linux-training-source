include ../Makefile.inc

GEN_EXE = orphan 

LINUX_EXE = \
	    cred_launcher \
	    demo_userns \
	    demo_uts_namespaces \
	    hostname \
	    multi_pidns \
	    ns_capable \
	    ns_child_exec \
	    ns_exec \
	    ns_run \
	    pidns_init_sleep \
	    show_creds \
	    simple_init \
	    t_setns_userns \
	    unshare \
	    userns_child_exec \
	    userns_setns_test

COURSE_FILES_EXE = ns_exec_map

EXERCISE_FILES_EXE = # ex.pidns_reparent ex.userns_cap_sig_expt

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

# Many of the programs here need libcap

LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBCAP}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
