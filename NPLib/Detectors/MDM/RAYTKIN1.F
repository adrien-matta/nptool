C****
C**** RAY TRACE  -  MIT VERSION 1984   (11/25/84) dy MOD 3/16/93
C****     add K=(1/p)dpdtheta to random rays 8/25/93  
C****     add kinematics program  3/7/96
C****  fix bug in random-kine (dele) 6/3/98, fix 2nd ray 6/4/98
C**** DR. STANLEY KOWALSKI
C**** MASS INST OF TECH
C**** BLDG 26-427
C**** CAMBRIDGE MASS 02139
C**** PH 617+253-4288
C****
      SUBROUTINE raytrace(NCTL)
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8   K
      LOGICAL LPLT   	
c	character*24 ctemp

C%%%% REAL*4 DAET, TYME
      COMMON  /BLCK00/  LPLT
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 1/  XI, YI, ZI, VXI, VYI, VZI, DELP
      COMMON  /BLCK 2/  XO, YO, ZO, VXO, VYO, VZO, RTL(1000), RLL(1000)
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK 6/  NP, JFOCAL
      COMMON  /BLCK 7/ NCODE
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK11/  EX, EY, EZ, QMC, IVEC
      COMMON  /DY1/ BDIPOLE
      COMMON  /kineblck/ THTSPEC,TRGT1,AM,QVALUE,EEXC,THETACAL,EKINE

c
C
C** xkine=K=(1/p)dp/dtheta
C** added by DY for spectrometer calculations 8/25/93
C
      COMMON  /BLCK15/TMIN,PMIN,XMAX,TMAX,YMAX,PMAX,DMAX,xkine
C
C%%%% DIMENSION DAET(3), TYME(2)
C*IBM DIMENSION DAET(5), TYME(2)
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
      DIMENSION XI(1000),YI(1000),ZI(1000),VXI(1000),VYI(1000),
     1    VZI(1000), DELP(1000)
      DIMENSION NWORD(15),DATA(75,200),IDATA(200),NTITLE(20),ITITLE(200)
      DIMENSION TC(6), DTC(6), R(6,6), T2(5,6,6)
      dimension am(4),theta(3),engy(3),rat(2),thetacal(10)
      DATA NWORD/4HSENT, 4HDIPO, 4HQUAD, 4HHEXA, 4HOCTA, 4HDECA, 4HEDIP,
     1   4HVELS, 4HPOLE, 4HMULT, 4HSHRT, 4HDRIF, 4HCOLL, 4HSOLE, 4HLENS/
      DATA C /3.D10/
C%%%% DATA TYME/4H    ,4H    /
      DATA NT1, NT2 /4H RT8,4H2.0 /
C****
C****
  100 FORMAT( 8F10.5 )
  101 FORMAT( 20A4 )
  102 FORMAT(10I5)
  103 FORMAT(///10X,' KEY WORD DOES NOT MATCH STORED LIST - NWD=',A4)
  104 FORMAT(//10X,' GO TO STATEMENT IN MAIN FELL THROUGH - I= ',I5,/)
  105 FORMAT( 1H1, 10X, 20A4  )
  106 FORMAT( 1H1 )
  107 FORMAT( 5F10.5/ 5F10.5/3F10.5/4F10.5/ 4F10.5/ 6F10.5/ 6F10.5/
     1        6F10.5/ 4F10.5/ 7F10.5/ 7F10.5                           )
  108 FORMAT('1',62X, 'RAY ', I4, //  30X, 'ENERGY=',F8.3,' MEV ', 7X,
     1   'PMOM=', F8.3, ' MEV/C', 6X, 'VELC=', 1PD11.3, ' CM/SEC'    /
     2   30X, 'DELE/E=', 0PF8.3, ' (PC)', 5X, 'DELP/P=', F8.3,
     3   ' (PC) ', 4X, 'DELV/V=', F7.3, '     (PC)'        /)
  109 FORMAT( 3F10.5/ 5F10.5/ 4F10.5/ 6F10.5/ 6F10.5                   )
  111 FORMAT( 2F10.5/ 6F10.5/ 2F10.5/ 6F10.5/ 3F10.5 )
  112 FORMAT( 3F10.5/ 4F10.5/ 5F10.5/ 4F10.5/ 6F10.5/ 6F10.5 / 8F10.5 )
  113 FORMAT( A4, 16X, A4  )
  114 FORMAT( 1F10.5 / 5F10.5 / 2F10.5  )
  115 FORMAT( 4F10.5/ 5F10.5/ 2F10.5/ 4F10.5/ 4F10.5/ 4F10.5/ 6F10.5/
     1   6F10.5/ 6F10.5/ 6F10.5   )
  116 FORMAT( /10X, '  PARTICLE ENERGY =', F10.4,  '  MEV'      /
     1         10X, 'PARTICLE MOMENTUM =', F10.4,  '  MEV/C'    /
     2         10X, 'PARTICLE VELOCITY =',1PD14.4, '  CM/SEC'  /
     3         10X, '             MASS =',0PF10.4, '  AMU'     /
     4         10X, '           CHARGE =', F10.4,  '  EQ'           )
C%%%% 117   FORMAT( 10X, 3A4, 1X, 2A4, I12, ' CPU.SEC'   )
C*IBM 117   FORMAT( 10X, 3A4, 1X, 2A4, 2A4 )
  118   FORMAT(4F10.5/5F10.5/F10.5/4F10.5/4F10.5/6F10.5/6F10.5)
  119   FORMAT( /// '  MAXIMUM NUMBER OF BEAM ELEMENTS EXCEEDED  ' /// )
C****
C%%%% CALL DATE(DAET)
C%%%% CALL TIME(TYME)
C*IBM CALL WHEN(DAET)
C**** CALL ERRSET( NUMBER, CONT, COUNT, TYPE, LOG, MAXLIN     )
C%%%% CALL ERRSET( 63, .TRUE., .FALSE., .FALSE., .FALSE., 2048)
C%%%% CALL ERRSET( 72, .TRUE., .FALSE., .FALSE., .TRUE.,  2560)
C%%%% CALL ERRSET( 74, .TRUE., .FALSE., .FALSE., .TRUE.,  2560)
C%%%% CALL ERRSET( 88, .TRUE., .FALSE., .FALSE., .TRUE.,  2560)
C%%%% CALL ERRSET( 89, .TRUE., .FALSE., .FALSE., .TRUE.,  2560)
C*IBM CALL ERRSET( 207, 256, 1 )
C*IBM CALL ERRSET( 208, 256, 1 )
C*IBM CALL ERRSET( 209, 256, 1 )
C*IBM CALL ERRSET( 210, 256, 1 )
C****
c      open(unit=6,file='output.dat',status='unknown')
      open(unit=5,file='rayin.dat',status='unknown')		
C****
      IF(NCTL.EQ.1 .OR. NCTL.EQ.2) THEN
        EMASS = PMASS*931.48
        ETOT = EMASS + ENERGY
        VEL = ( DSQRT( ( 2.*EMASS + ENERGY)*ENERGY) / ETOT ) * C
        VEL0 = VEL
        EN0 = ENERGY
        PMOM0 = DSQRT( (2.*EMASS + EN0)*EN0)
        IF(NCTL.EQ.1) GOTO 66
        IF(NCTL.EQ.2) GOTO 52
      ENDIF
    5 LPLT = .FALSE.
      IVEC = 0
      LNEN = 0
      NMAX = 200
      DO 1  I=1,NMAX
      IDATA(I)= 0
      DO 1  J=1,75
      DATA(J,I) = 0.
    1 CONTINUE
      READ ( 5,101,END=99) NTITLE
      NTITLE(19) = NT1
      NTITLE(20) = NT2
      READ (5,*)NR, IP, NSKIP, JFOCAL, JMTRX, JNR, NPLT
      READ (5,100) ENERGY, DEN, XNEN, PMASS, Q0
      IF( NPLT .NE. 0 ) LPLT = .TRUE.
      IF( NR .GT. 1000) NR=1000
      IF( Q0 .EQ. 0. )  Q0 = 1.
      EMASS = PMASS*931.48
      QMC = EMASS/(9.D10*Q0)
      ETOT = EMASS + ENERGY
      VEL = ( DSQRT( ( 2.*EMASS + ENERGY)*ENERGY) / ETOT ) * C
      VEL0 = VEL
      EN0 = ENERGY
      PMOM0 = DSQRT( (2.*EMASS + EN0)*EN0)
      NEN = XNEN
      IF( NEN  .EQ.  0 ) NEN = 1
      NO = 1
    2 IF( NO .LE. NMAX ) GO TO 6
C      WRITE(6,119)
      CALL EXIT
    6 READ (5,113) NWD, ITITLE(NO)
      DO 3  I=1,15
      IF( NWD  .EQ. NWORD(I) ) GO TO 4
    3 CONTINUE
C      WRITE(6,103) NWD
c   99 CALL EXIT
   99 RETURN  
    4 GO TO( 11, 12, 13, 13, 13, 13, 17, 18, 19, 20,21,22,23,24,25), I
C****
C****
C****
C****
C      WRITE(6, 104)  I
      CALL EXIT
C****
C**** DIPOLE  LENS           TYPE = 2
C****
   12 IDATA(NO) = 2
      READ (5,107) ( DATA( J,NO ) , J=1,5 ), ( DATA( J,NO ), J=11,22 ),
     1          ( DATA( J,NO ) , J=25,64)
      BDIPOLE=DATA(15,NO)
      NO = NO + 1
      GO TO 2
C****
C**** PURE MULTIPOLES
C**** QUADRUPOLE LENS        TYPE = 3
C**** HEXAPOLE  LENS         TYPE = 4
C**** OCTAPOLE  LENS         TYPE = 5
C**** DECAPOLE  LENS         TYPE = 6
C****
   13 IDATA(NO) = I
      READ (5,109)( DATA( J,NO ) , J=1,3 ), ( DATA( J,NO ), J=10,30 )
      NO = NO + 1
      GO TO 2
C****
C****   ELECTROSTATIC DEFLECTOR  TYPE=7
C****
   17 IDATA(NO) = 7
      READ(5,118) (DATA(J, NO), J=1, 4), (DATA(J, NO), J=11,20),
     1              (DATA(J, NO), J=25,40)
        NO = NO + 1
        GO TO 2
C****
C**** VELOCITY SELECTOR      TYPE = 8
C****
   18 IDATA(NO) = 8
      READ (5,115) ( DATA(J,NO),J=1,4), (DATA(J,NO), J=7,11 ),
     1             ( DATA(J,NO),J=12,13),(DATA(J,NO),J=16,51)
      NO = NO + 1
      GO TO 2
C****
C**** MULTIPOLE (POLES)      TYPE =  9
C****
   19 IDATA(NO) = 9
      READ (5,112) ( DATA( J,NO ) , J=1,3 ), ( DATA( J,NO ), J=10,34 ),
     1             ( DATA( J,NO ) , J=35,42)
      NO = NO + 1
      GO TO 2
C****
C**** MULTIPOLE LENS         TYPE = 10
C****
   20 IDATA(NO) = 10
      READ (5,111) ( DATA( J,NO ) , J=1,2 ), ( DATA( J,NO ), J=10,17 ),
     1          ( DATA( J,NO ) , J=20,28 )
      NO = NO + 1
      GO TO 2
C****
C**** SHIFT AND ROTATE       TYPE = 11
C****
   21 IDATA(NO) = 11
      READ (5,100) ( DATA( J,NO ) , J=1,6 )
      NO = NO + 1
      GO TO 2
C****
C**** DRIFT                  TYPE = 12
C****
   22 IDATA(NO) = 12
      READ (5,100) ( DATA( J,NO ) , J=1,1 )
      NO = NO + 1
      GO TO 2
C****
C**** COLLIMATOR             TYPE = 13
C****
   23 IDATA(NO) = 13
      READ(5,100)  (DATA(J,NO),J=1,5)
      NO = NO+1
      GO TO 2
C****
C**** SOLENOID               TYPE = 14
C****
   24 IDATA(NO) = 14
      READ (5,114) (DATA(J,NO),J=1,1), ( DATA(J,NO), J=10,16)
      NO = NO+1
      GO TO 2
C****
C**** LENS                   TYPE = 15
C****
   25 IDATA(NO) = 15
      READ (5,100) (DATA(J,NO), J=1,9  )
      NO = NO+1
      GO TO 2
C****
C**** SYSTEM END             TYPE = 1
C****
   11 IDATA(NO) = 1
C****
C**** STANDARD RAYS AUTOMATIC SET-UP
C**** IF( NR .GT. JNR ) APPEND ADDITIONAL RAYS FROM INPUT
C****
      RETURN
      IF (JNR.EQ.0) GO TO 66
C
C** xkine=K=(1/p)dp/dtheta
C** added by DY for spectrometer calculations 8/25/93
C
      READ (5,100) TMIN,PMIN,XMAX,TMAX,YMAX,PMAX,DMAX,xkine
      CALL RAYS(JNR)
      IF( JNR .GE. NR ) GO TO 52
      JNRP = JNR+1
C**********************************************************************
C**********************************************************************
      if(jmtrx.eq.1) then
      read(5,100) xmax1,tmax1,ymax1,pmax1,dmax1
         open (unit=10,file='RANDOM.NUM',
     1    status='unknown')
         do 4900 j=jnrp,nr
         read(10,100) xi(j),vxi(j),yi(j),vyi(j),zi(i),vzi(i),delp(j)
         xi(j)=(xi(j)-0.5)*xmax1
         vxi(j)=(vxi(j)-0.5)*tmax1
         yi(j)=(yi(j)-0.5)*ymax1
         vyi(j)=(vyi(j)-0.5)*pmax1
         zi(j)=0
         vzi(j)=0
         delp(j)=(delp(j)-0.5)*dmax1
C
C** add correlation of theta and energy for spectrometer
C
         delp(j)=delp(j)-xkine*vxi(j)*0.2
C
 4900    continue
      endif
      close(unit=10)
      if(jmtrx.eq.1) go to 52
C**********************************************************************
C***************************************************************  TMC *
C****  Random rays with kinematics and angle offset              
C**********************************************************************
C***** toff1 is angle relative to central ray  *************
      if(jmtrx.eq.2) then
      read(5,100) xmax1,tmax1,ymax1,pmax1,dmax1
         open (unit=10,file='RANDOM.NUM',
     1    status='unknown')
         do 4910 j=jnrp,nr
         read(10,100) xi(j),vxi(j),yi(j),vyi(j),zi(i),vzi(i),delp(j)
         xi(j)=(xi(j)-0.5)*xmax1
         vxi(j)=(vxi(j)-0.5)*tmax1
         yi(j)=(yi(j)-0.5)*ymax1
         vyi(j)=(vyi(j)-0.5)*pmax1
         zi(j)=0
         vzi(j)=0
         delp(j)=(delp(j)-0.5)*dmax1
4910     continue
C
C** 
C
C
      read (5,100) THTSPEC,THTSCAT,AM(1),AM(2),QVALUE,EEXC
        ENGY(1)=QVALUE-EEXC 
        ENGY(2)=ENERGY
        AM(3)=PMASS
        THETA(1)=THTSPEC 
        CALL KINE (AM,THETA,ENGY,RAT,PATR,KB)
        ENERGY=ENGY(3) 
        ETOT = EMASS + ENERGY
        VEL = ( DSQRT( ( 2.*EMASS + ENERGY)*ENERGY) / ETOT ) * C
        VEL0 = VEL
        EN0 = ENERGY
        PMOM0 = DSQRT( (2.*EMASS + EN0)*EN0)
        THTSPEC=THTSPEC*17.45329
        THTSCAT=THTSCAT*17.45329
        DO 4915 J=2,NR
C****   DO 4915 J=JNRP,NR   ** THIS WAS CHANGED SO 2ND RAY HAS PROPER KINEMATICS
C****                          to get focal plane position.  
        VXI(J)=VXI(J)-THTSPEC+THTSCAT 
        THETA(1)= SQRT((THTSCAT+VXI(J))**2+VYI(J)**2)
        THETA(1)=THETA(1)/17.45329 
        CALL KINE (AM,THETA,ENGY,RAT,PATR,KB)
        DELE=100*(ENGY(3)-ENERGY)/(ENERGY)    
        DELP(J)=DELP(J)+DELE
 4915   continue
      endif
      close(unit=10)
      if(jmtrx.eq.2) go to 52
C**********************************************************************
C***************************************************************  TMC *
      DO 49 J=JNRP,NR
   49 READ(5,100,END=60) XI(J),VXI(J),YI(J),VYI(J),ZI(J),VZI(J),
     1                    DELP(J)
      GO TO 52
C****
C**** INPUT RAYS
C****
   66 IF (JMTRX.NE.3) GO TO 4940 
C*   added 3/6/96 DHY to use KINE to get ray energies 
c       read (5,100) THTSPEC,TRGT1,AM(1),AM(2),QVALUE,EEXC,FNANG
C       write (6,100) THTSPEC,TRGT1,AM(1),AM(2),QVALUE,EEXC,FNANG
C	read (*,*)
        FNANG=1
        EBEAM=ENERGY
        ENGY(1)=QVALUE-EEXC 
        ENGY(2)=EBEAM-TRGT1
        AM(3)=PMASS
        THETA(1)=THTSPEC 
c 	write (6,100)am(1),am(2),
c     1 		engy(1),engy(2),engy(3),pmass
        CALL KINE (AM,THETA,ENGY,RAT,PATR,KB)
C	write (6,1001) am(1),am(2),engy(1),engy(2),engy(3),pmass
C	read (*,*)
1001    format (' after kine', 6F10.4)
        ENERGY=ENGY(3) 
        ETOT = EMASS + ENERGY
        VEL = ( DSQRT( ( 2.*EMASS + ENERGY)*ENERGY) / ETOT ) * C
        VEL0 = VEL
        EN0 = ENERGY
        EKINE = ENERGY
        PMOM0 = DSQRT( (2.*EMASS + EN0)*EN0)
        NANG=FNANG
        NCASE=NR/FNANG
C        READ (5,4945) (THETACAL(J),J=1,NANG)
C	write (*,*) nr,(thetacal(j),j=1,NANG)
 4945  format (10F7.3)
      DO 4952 JJ=1,NCASE
      JN=JJ*NANG-NANG+1
      DO 4950 J=JN,JN+NANG-1
      XI(J)=0.
      YI(J)=0.
      VYI(J)=0.
      ZI(J)=0.
      VZI(J)=0.
      THETA(1)=THETACAL(J-JN+1)
c 	write (6,100) am(1),am(2),engy(1),engy(2),engy(3)
        CALL KINE (AM,THETA,ENGY,RAT,PATR,KB)
c 	write (6,1001)am(1),am(2),engy(1),engy(2),engy(3)
        DELP(J)=100*(ENGY(3)-ENERGY)/(ENERGY)   
        VXI(J)=THETACAL(J-JN+1)-THTSPEC
        VXI(J)=VXI(J)*17.45329 
 4950 CONTINUE         
c	write (*,*) ' after 4950'
c       read (5,100) THTSPEC,TRGT2,AM(1),AM(2),QVALUE,EEXC
C       write (6,100) THTSPEC,TRGT2,AM(1),AM(2),QVALUE,EEXC
C	read (*,*)
        ENGY(1)=QVALUE-EEXC 
        ENGY(2)=EBEAM-TRGT2
 4952  CONTINUE        
      GO TO 52
C  ***********
C  *********** end of addition to use KINE to get RAY entries
C  *******************
 4940 DO 56  J=1,NR
      READ(5,100,END=60 )XI(J),VXI(J),YI(J),VYI(J),ZI(J),VZI(J),DELP(J)
   56 CONTINUE
      GO TO 52
   60 NR = J-1
   52 DO 53 JEN=1,NEN
C****
C****
C****
      NP = IP
      IF( (NP .LE. 100)  .OR.  (NP .GE. 200)  ) GO TO 65
      IF( JEN   .EQ.   (NEN/2+1)  )  NP = IP-100
   65 CONTINUE
      IF( (NP .GT. 100)  .AND.  (JEN .NE. 1) )  GO TO 55
c      WRITE(6, 105) NTITLE
C%%%% WRITE(6, 117) DAET, TYME
c%%%%
c     call dtime
c	ctemp=ctime(time())	 
c	write(6,*) ctemp

c%%%%
c      WRITE(6, 116) EN0, PMOM0, VEL0, PMASS, Q0
      DO 54  NO = 1,200
      ITYPE = IDATA(NO)
      IF( ITYPE .EQ. 1 ) GO TO 55
   54 CALL PRNT( ITYPE, NO )
   55 CONTINUE
C**** IF( ( NP .GT. 100) .AND. (JEN .EQ. 1 ) ) WRITE(6, 106)
      DO 57  J=1,NR
c%%%  ibm pc statements %%%%%%%%%%%%%%
c      write(0,5710) j
 5710 format (i5)
c%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      ENERGY = (1.+DELP(J)/100. ) *EN0
      ETOT = EMASS + ENERGY
      VEL = ( DSQRT( (2.*EMASS + ENERGY) *ENERGY) /ETOT)*C
      PMOM =  DSQRT( (2.*EMASS + ENERGY) *ENERGY)
      K = (Q0/ETOT)*9.D10
C****
      T = 0.
      NUM = 0
      XA = XI(J)
      YA = YI(J)
      ZA = ZI(J)
      VXA =VEL*DSIN( VXI(J)/1000. ) * DCOS( VYI(J)/1000. )
      VYA =VEL*DSIN( VYI(J)/1000. )
      VZA =VEL*DCOS( VXI(J)/1000. ) * DCOS( VYI(J)/1000. )
      XDVEL = (VEL-VEL0)*100./VEL0
      DELTP = (PMOM-PMOM0)*100./PMOM0
c      IF( NP .LE. 100) WRITE(6,108)J,ENERGY,PMOM,VEL,DELP(J),DELTP,XDVEL
      DO 50 NO =1,200
      ITYPE = IDATA(NO )
      GO TO( 31,32,33,33,33,33,37,38,39,40,41,42,46,44,45)      ,ITYPE
      CALL EXIT
C****
C****
   32 CALL DIPOLE ( NO, NP, T, TP ,NUM )
      GO TO 51
   33 NCODE = ITYPE-2
      CALL MULTPL ( NO, NP, T, TP ,NUM )
      GO TO 51
   37 IVEC = 1
      CALL EDIPL(NO, NP, T, TP, NUM)
      IVEC = 0
      GO TO 51
   38 IVEC = 1
      CALL VELS   ( NO, NP, T, TP ,NUM )
      IVEC = 0
      GO TO 51
   39 CALL POLES  ( NO, NP, T, TP ,NUM )
      GO TO 51
   40 CALL MULT   ( NO, NP, T, TP ,NUM )
      GO TO 51
   41 CALL SHROT  ( NO, NP, T, TP ,NUM )
      GO TO 50
   42 CALL DRIFT  ( NO, NP, T, TP ,NUM )
      GO TO 50
   44 CALL SOLND  ( NO, NP, T, TP ,NUM )
      GO TO 51
   45 CALL LENS   ( NO, NP, T, TP ,NUM )
      GO TO 50
   46 CALL COLL   ( NO,  J, IFLAG      )
      IF( IFLAG .NE. 0 ) GO TO 57
      GO TO 50
   51 XA = TC(1)
      YA = TC(2)
      ZA = TC(3)
      VXA= TC(4)
      VYA= TC(5)
      VZA= TC(6)
   50 CONTINUE
   31 CONTINUE
      CALL OPTIC( J, JFOCAL, NP, T, TP )
      IF (LPLT ) CALL PLTOUT ( JEN, J, NUM )
   57 CONTINUE
      ENERGY = EN0
      VEL = VEL0
      IF( NP .GT. 100 ) GO TO 59
C      WRITE(6, 105) NTITLE
C%%%% WRITE(6, 117) DAET,TYME
C%%%%
C%%%% call dtime
c%%%%
C      WRITE(6, 116) EN0, PMOM0, VEL0, PMASS, Q0
      DO 58 NO =1,200
      ITYPE = IDATA(NO )
      IF ( ITYPE  .EQ.  1 ) GO TO 59
   58 CALL PRNT( ITYPE, NO )
   59 CONTINUE
      IF( NSKIP .NE. 0 ) GO TO 61
      IF( NR  .GE.  46  )  GO TO 62
      IF( NR  .GE.  14  )  GO TO 63
      IF( NR  .GE.   6  )  GO TO 64
      GO TO 61
   62 CALL MATRIX(R,T2)
      GO TO 61
   63 CONTINUE
c   63 WRITE(6, 105) NTITLE
C%%%% ICPU = ITCPU( )/100
C%%%% WRITE(6, 117) DAET, TYME, ICPU
C%%%%
C%%%% call dtime
C%%%%
      CALL MTRX1( 0, JEN, NR, ENERGY  )
      LNEN = 1
      GO TO 61
   64 CONTINUE
c   64 WRITE(6, 105) NTITLE
C%%%% ICPU = ITCPU( )/100
C%%%% WRITE(6, 117) DAET, TYME, ICPU
C%%%%
C%%%% call dtime
C%%%%
      CALL MTRX1( 1, JEN, NR, ENERGY )
      LNEN = 1
   61 CALL PRNT1 ( NR )
      EN0 = EN0 + DEN
      ENERGY = EN0
      ETOT = EMASS + EN0
      VEL0 = ( DSQRT( ( 2.*EMASS + EN0)*EN0 ) /ETOT)*C
      PMOM0 = DSQRT( (2.*EMASS + EN0)*EN0)
   53 CONTINUE
c      IF(  (LNEN .EQ. 0 )  .OR.   (NEN .EQ. 1 )  )  GO TO 5
      IF(  (LNEN .EQ. 0 )  .OR.   (NEN .EQ. 1 )  ) THEN
      close(5) 
      RETURN
      ENDIF
c      WRITE(6, 105) NTITLE
C**** CALL TIME(TYME)
C%%%% ICPU = ITCPU( )/100
C%%%% WRITE(6, 117) DAET, TYME, ICPU
C%%%%
C%%%% call dtime
C%%%%
C*IBM CALL WHEN(DAET)
      CALL MPRNT( NEN )
c      WRITE(6, 106)
      GO TO 5
      END


      SUBROUTINE MATRIX( R, T2  )
C****
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 1/  XI, YI, ZI, VXI, VYI, VZI, DELP
      COMMON  /BLCK 2/  XO, YO, ZO, VXO, VYO, VZO,RTL(1000),RLL(1000)
      DIMENSION XI(1000),YI(1000),ZI(1000),VXI(1000),VYI(1000),
     1        VZI(1000),DELP(1000)
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
      DIMENSION  R(6,6) , T2(5,6,6), TT(5,6,6)
      DO 21  I1= 1,6
      DO 21  I2= 1,6
      R(I1,I2) = 0.
      DO 21 I3= 1,5
   21 T2(I3,I1,I2) = 0.
C****
C****
C**** CALCULATE COEFFICIENTS
C****
      R(1,1) =  ( XO(3) -  XO(4) ) / ( XI(3) -  XI(4) )
      R(1,2) =  ( XO(5) -  XO(6) ) / (VXI(5) - VXI(6) )
      R(1,3) =  ( XO(7) -  XO(8) ) / ( YI(7) -  YI(8) )
      R(1,4) =  ( XO(9) -  XO(10)) / (VYI(9) - VYI(10))
      R(1,6) =  ( XO(11)-  XO(12) )/ (DELP(11) - DELP(12) )
      R(2,1) =  (VXO(3) - VXO(4) ) / ( XI(3) -  XI(4) )
      R(2,2) =  (VXO(5) - VXO(6) ) / (VXI(5) - VXI(6) )
      R(2,3) =  (VXO(7) - VXO(8) ) / ( YI(7) -  YI(8) )
      R(2,4) =  (VXO(9) - VXO(10)) / (VYI(9) - VYI(10))
      R(2,6) =  (VXO(11)- VXO(12) )/ (DELP(11) - DELP(12) )
      R(3,1) =  ( YO(3) -  YO(4) ) / ( XI(3) -  XI(4) )
      R(3,2) =  ( YO(5) -  YO(6) ) / (VXI(5) - VXI(6) )
      R(3,3) =  ( YO(7) -  YO(8) ) / ( YI(7) -  YI(8) )
      R(3,4) =  ( YO(9) -  YO(10)) / (VYI(9) - VYI(10))
      R(3,6) =  ( YO(11)-  YO(12) )/ (DELP(11) - DELP(12) )
      R(4,1) =  (VYO(3) - VYO(4) ) / ( XI(3) -  XI(4) )
      R(4,2) =  (VYO(5) - VYO(6) ) / (VXI(5) - VXI(6) )
      R(4,3) =  (VYO(7) - VYO(8) ) / ( YI(7) -  YI(8) )
      R(4,4) =  (VYO(9) - VYO(10)) / (VYI(9) - VYI(10))
      R(4,6) =  (VYO(11)- VYO(12) )/ (DELP(11) - DELP(12) )
      R( 5,5 )  =  1.
      R( 6,6 )  =  1.
      R(5,1) =  (RTL(3) - RTL(4) ) / ( XI(3) -  XI(4) )
      R(5,2) =  (RTL(5) - RTL(6) ) / (VXI(5) - VXI(6) )
      R(5,6) =  (RTL(11)- RTL(12) )/ (DELP(11) - DELP(12) )
C****
C****
      T2(1,1,1)= ( XO(3) + XO(4) ) /(2.*XI(3)**2 )
      T2(1,2,2)= ( XO(5) + XO(6) ) /(2.*VXI(5)**2)
      T2(1,3,3)= ( XO(7) + XO(8) ) /(2.*YI(7)**2 )
      T2(1,4,4)= ( XO(9) + XO(10) ) /(2.*VYI(9)**2 )
      T2(1,6,6)= ( XO(11) + XO(12) ) /(2.*DELP(11)**2 )
      T2(1,1,2)= ( XO(13)+XO(14)-2.*T2(1,1,1)*XI(13)**2-2.*T2(1,2,2)*
     1   VXI(13)**2 ) /(2.*XI(13)*VXI(13) )
      T2(1,1,6)= ( XO(15) + XO(16) -2.*T2(1,1,1)*XI(15)**2 -
     1  2.*T2(1,6,6)*DELP(15)**2 ) /(2.*XI(15)*DELP(15) )
      T2(1,2,6)= ( XO(17) + XO(18) -2.*T2(1,2,2)*VXI(17)**2 -
     1  2.*T2(1,6,6)*DELP(17)**2 ) /(2.*VXI(17)*DELP(17) )
      T2(1,3,4)= ( XO(19)- XO(20) ) /(2.*YI(19)*VYI(19) )
      T2(2,1,1)= (VXO(3) +VXO(4) ) /(2.*XI(3)**2 )
      T2(2,2,2)= (VXO(5) +VXO(6) ) /(2.*VXI(5)**2)
      T2(2,3,3)= (VXO(7) +VXO(8) ) /(2.*YI(7)**2 )
      T2(2,4,4)= (VXO(9) +VXO(10) ) /(2.*VYI(9)**2 )
      T2(2,6,6)= (VXO(11) +VXO(12) ) /(2.*DELP(11)**2 )
      T2(2,1,2)=(VXO(13)+VXO(14)-2.*T2(2,1,1)*XI(13)**2-2.*T2(2,2,2)*
     1   VXI(13)**2 ) /(2.*XI(13)*VXI(13) )
      T2(2,1,6)= (VXO(15) +VXO(16) -2.*T2(2,1,1)*XI(15)**2 -
     1  2.*T2(2,6,6)*DELP(15)**2 ) /(2.*XI(15)*DELP(15) )
      T2(2,2,6)= (VXO(17) +VXO(18) -2.*T2(2,2,2)*VXI(17)**2 -
     1  2.*T2(2,6,6)*DELP(17)**2 ) /(2.*VXI(17)*DELP(17) )
      T2(2,3,4)= (VXO(19)-VXO(20) ) /(2.*YI(19)*VYI(19) )
      T2(3,1,3)= ( YO(21) - YO(22) ) /(2.*XI(21)*YI(21) )
      T2(3,1,4)= ( YO(23) - YO(24) ) /(2.*XI(23)*VYI(23) )
      T2(3,2,3)= ( YO(25) - YO(26) ) /(2. *VXI(25)*YI(25) )
      T2(3,2,4)= ( YO(27) - YO(28) ) /(2.*VXI(27)*VYI(27) )
      T2(3,3,6)= ( YO(29) - YO(30) ) /(2.*YI(29)*DELP(29) )
      T2(3,4,6)= ( YO(31) - YO(32) ) /(2.*VYI(31)*DELP(31)  )
      T2(4,1,3)= (VYO(21) -VYO(22) ) /(2.*XI(21)*YI(21) )
      T2(4,1,4)= (VYO(23) -VYO(24) ) /(2.*XI(23)*VYI(23) )
      T2(4,2,3)= (VYO(25) -VYO(26) ) /(2. *VXI(25)*YI(25) )
      T2(4,2,4)= (VYO(27) -VYO(28) ) /(2.*VXI(27)*VYI(27) )
      T2(4,3,6)= (VYO(29) -VYO(30) ) /(2.*YI(29)*DELP(29) )
      T2(4,4,6)= (VYO(31) -VYO(32) ) /(2.*VYI(31)*DELP(31)  )
C****
C**** PATH LENGTH TERMS
C****
      T2(5,1,1) = ( RTL(3) + RTL(4) - 2*RTL(1) ) /( 2* XI(3)**2 )
      T2(5,2,2) = ( RTL(5) + RTL(6) - 2*RTL(1) ) /( 2*VXI(5)**2 )
      T2(5,3,3) = ( RTL(7) + RTL(8) - 2*RTL(1) ) /( 2* YI(7)**2 )
      T2(5,4,4) = ( RTL(9) + RTL(10)- 2*RTL(1) ) /( 2*VYI(9)**2 )
      T2(5,6,6) = ( RTL(11)+ RTL(12)- 2*RTL(1) ) /( 2*DELP(11)**2 )
      T2(5,1,2) = ( RTL(13)+ RTL(14)- 2*RTL(1) - 2*T2(5,1,1)* XI(13)**2-
     1            2*T2(5,2,2)*VXI(13)**2 ) / ( 2* XI(13)*VXI(13) )
      T2(5,1,6) = ( RTL(15)+ RTL(16)- 2*RTL(1) - 2*T2(5,1,1)* XI(15)**2-
     1            2*T2(5,6,6)*DELP(15)**2) / ( 2* XI(15)*DELP(15))
      T2(5,2,6) = ( RTL(17)+ RTL(18)- 2*RTL(1) - 2*T2(5,2,2)*VXI(17)**2-
     1            2*T2(5,6,6)*DELP(17)**2) / ( 2*VXI(17)*DELP(17))
      T2(5,3,4) = ( RTL(19)- RTL(20)           ) /( 2* YI(19)*VYI(19) )
C****
C****
c      write (6, 22)  ( ( R(IR, IJ), IJ=1,6), IR=1,6)
   22 FORMAT(1H1, / 51X, 15H *TRANSFORM* 1  ,  / 6(25X, 6F10.5/)  )
c      write (6, 120)
  120 FORMAT(   /46X, 25H  *2ND ORDER TRANSFORM*           )
      DO 24 I1= 1,5
      DO 25 I2= 1,6
c      write (6, 121) ( I1,I3,I2, T2(I1,I3,I2), I3=1,I2 )
  121 FORMAT( 6(I4,I2,I1, 1PE11.3)  )
   25 CONTINUE
c      write (6, 122)
  122 FORMAT( 1H  )
   24 CONTINUE
      XTTT=((XO(33)- XO(34) )/2. - R(1,2)*VXI(33) )/VXI(33)**3
      XTPP  = (XO(27) - XO(28) + XO(6) -XO(5))/(2.*VXI(27)*VYI(27)**2)
      XXTT  = (XO(37) - XO(36) + XO(35)-XO(38)- 2.*(XO( 3) - XO( 4) ) )/
     1   (4.*XI(35) * VXI(35)**2 )
      XXXT  = (XO(35) - XO(37) + XO(36)-XO(38)- 2.*(XO(33) - XO(34) ) )/
     1   (4.*XI(35)**2*VXI(35))
      XTTD  = (XO(39) - XO(40) + XO(41)-XO(42)- 2.*(XO(11) - XO(12) ) )/
     1   (4.*VXI(39)**2*DELP(39))
      XTDD  = (XO(39) - XO(41) + XO(40)-XO(42)- 2.*(XO(33) - XO(34) ) )/
     1   (4.*VXI(39)*DELP(39)**2)
      XXPP  = (XO(23) - XO(24) + XO( 4)-XO( 3))/(2.*XI(23)*VYI(23)**2  )
      XPPD  = (XO(31) - XO(32) + XO(12)-XO(11))/(2.*VYI(31)**2*DELP(31))
      XTTTT=((XO(33)+XO(34) )/2. - T2(1,2,2)*VXI(33)**2)/ VXI(33)**4
      XTTPP = (XO(27) - XO( 5) + XO(28)-XO( 6) - 2.*XO( 9) ) /
     1   ( 2.*VXI(27)**2*VYI(27)**2 )
      XPPDD = (XO(31) - XO(11) + XO(32)-XO(12) - 2.*XO( 9) ) /
     1   ( 2.*VYI(31)**2 * DELP(31)**2 )
      XPPPP =(XO(43) -T2(1,4,4)*VYI(43)**2) / VYI(43)**4
      ZDDD = ( (RTL(45) - RTL(46) )/2. - R(5,6)*DELP(45) )/DELP(45)**3
      ZDDDD = ( (RTL(45)+RTL(46)-2*RTL(1) )/2. -T2(5,6,6)*DELP(45)**2)/
     1   DELP(45)**4
      XDDD = (( XO(45)- XO(46))/2. - R(1,6)*DELP(45) ) / DELP(45)**3
      XDDDD= (( XO(45)+ XO(46))/2. - T2(1,6,6)*DELP(45)**2 )/DELP(45)**4
      TDDD = ((VXO(45)-VXO(46))/2. - R(2,6)*DELP(45) ) / DELP(45)**3
      TDDDD= ((VXO(45)+VXO(46))/2. - T2(2,6,6)*DELP(45)**2 )/DELP(45)**4
c      write (6, 26) XTTT, XTPP, XXTT, XXXT, XTTD, XTDD, XXPP, XPPD,
c     1   XTTTT, XTTPP, XPPDD, XPPPP,
c     2   XDDD, XDDDD, TDDD, TDDDD,      ZDDD, ZDDDD
   26 FORMAT('1',/15X, 'X/THETA**3       =',1PE11.3   /
     1            15X, 'X/THETA.PHI**2   =',1PE11.3   /
     2            15X, 'X/X.THETA**2     =',1PE11.3   /
     3            15X, 'X/X**2.THETA     =',1PE11.3   /
     4            15X, 'X/THETA**2.DELTA =',1PE11.3   /
     5            15X, 'X/THETA.DELTA**2 =',1PE11.3   /
     6            15X, 'X/X.PHI**2       =',1PE11.3   /
     7            15X, 'X/PHI**2.DELTA   =',1PE11.3   //
     8            15X, 'X/THETA**4       =',1PE11.3   /
     9            15X, 'X/THETA**2.PHI**2=',1PE11.3   /
     A            15X, 'X/PHI**2.DELTA**2=',1PE11.3   /
     B            15X, 'X/PHI**4         =',1PE11.3   //
     C            15X, 'X/DELTA*3        =',1PE11.3   /
     D            15X, 'X/DELTA*4        =',1PE11.3   /
     E            15X, 'THETA/DELTA*3    =',1PE11.3   /
     F            15X, 'THETA/DELTA*4    =',1PE11.3   /
     H            15X, 'Z/DELTA*3        =',1PE11.3   /
     I            15X, 'Z/DELTA*4        =',1PE11.3   )
C
C***********************************************************************
C THE NEXT LINE TURNS OFF EXTRA FOCAL PLANE CALC'S
        RETURN
C***********************************************************************
      DO 1  I1=1,5
      DO 1  I2=1,6
      DO 1  I3=1,6
    1 TT(I1,I2,I3) = T2(I1,I2,I3)
      DO 2 I=1,12
      PSI =  5. * FLOAT(I)
      TPSI = .001*DTAN( PSI/57.29578 )
      TT(1,1,1) = T2(1,1,1) + R(2,1) * R(1,1) * TPSI
      TT(1,1,2) = T2(1,1,2) + ( R(2,1)*R(1,2) + R(2,2)*R(1,1) ) * TPSI
      TT(1,2,2) = T2(1,2,2) + R(2,2) * R(1,2) * TPSI
      TT(1,1,6) = T2(1,1,6) + ( R(2,1)*R(1,6) + R(2,6)*R(1,1) ) * TPSI
      TT(1,2,6) = T2(1,2,6) + ( R(2,2)*R(1,6) + R(2,6)*R(1,2) ) * TPSI
      TT(1,6,6) = T2(1,6,6) + R(2,6) * R(1,6) * TPSI
      TT(3,1,3) = T2(3,1,3) + R(1,1) * R(4,3) * TPSI
      TT(3,1,4) = T2(3,1,4) + R(1,1) * R(4,4) * TPSI
      TT(3,2,3) = T2(3,2,3) + R(1,2) * R(4,3) * TPSI
      TT(3,2,4) = T2(3,2,4) + R(1,2) * R(4,4) * TPSI
      TT(3,3,6) = T2(3,3,6) + R(1,6) * R(4,3) * TPSI
      TT(3,4,6) = T2(3,4,6) + R(1,6) * R(4,4) * TPSI
      CTTT=XTTT+ ( R(1,2)*T2(2,2,2) + R(2,2)*T2(1,2,2) ) * TPSI
      CTPP=XTPP+ ( R(1,2)*T2(2,4,4) + R(2,2)*T2(1,4,4) ) * TPSI
      CXTT=XXTT+ ( R(1,1)*T2(2,2,2) + R(1,2)*T2(2,1,2) +
     1             R(2,1)*T2(1,2,2) + R(2,2)*T2(1,1,2) ) * TPSI
      CXXT=XXXT+ ( R(1,1)*T2(2,1,2) + R(1,2)*T2(2,1,1) +
     1             R(2,1)*T2(1,1,2) + R(2,2)*T2(1,1,1) ) * TPSI
      CTTD=XTTD+ ( R(1,2)*T2(2,2,6) + R(1,6)*T2(2,2,2) +
     1             R(2,2)*T2(1,2,6) + R(2,6)*T2(1,2,2) ) * TPSI
      CTDD=XTDD+ ( R(1,2)*T2(2,6,6) + R(1,6)*T2(2,2,6) +
     1             R(2,2)*T2(1,6,6) + R(2,6)*T2(1,2,6) ) * TPSI
      CXPP=XXPP+ ( R(1,1)*T2(2,4,4) + R(2,1)*T2(1,4,4) ) * TPSI
      CPPD=XPPD+ ( R(1,6)*T2(2,4,4) + R(2,2)*T2(1,4,4) ) * TPSI
c      write (6, 27) PSI
   27 FORMAT(1H1, 35X,'FOCAL PLANE TILT ANGLE= ',F07.2, '   DEGREES '  )
c      write (6, 28)  ( ( R(IR, IJ), IJ=1,6), IR=1,6)
   28 FORMAT(     / 51X, 15H *TRANSFORM* 1  ,  / 6(25X, 6F10.5/)  )
c      write (6, 120)
      DO 29 I1= 1,5
      DO 30 I2= 1,6
c      write (6, 121) ( I1,I3,I2, TT(I1,I3,I2), I3=1,I2 )
   30 CONTINUE
c      write (6, 122)
   29 CONTINUE
c      write (6, 26) CTTT, CTPP, CXTT, CXXT, CTTD, CTDD, CXPP, CPPD,
c     1   XTTTT, XTTPP, XPPDD, XPPPP,
c     2   XDDD, XDDDD, TDDD, TDDDD,      ZDDD, ZDDDD
    2 CONTINUE
      RETURN
      END



      SUBROUTINE  MLTT ( BFLD, Z, X, Y )
C****
C****
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  K, L
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLK100/  W, L, D, DG, S, BF, BT
      COMMON  /BLK101/  C0, C1, C2, C3, C4, C5, C6, C7, C8
      DIMENSION TC(6), DTC(6)
      U = 2.*X/W
      S = 2.*Z/L
      DL2 = (L/D)**2
      W1 = C0 + C1*U + C2*U*U + C3*U**3 + C4*U**4 + C5*U**5
      W2 = 1. + C7*( S**4 + DL2*C8*S**8 ) / ( 1. + DL2*C8 )
      BFLD = BF*W1 / W2
      RETURN
      END


      SUBROUTINE MTRX1( M, JEN, NR, ENERGY  )
C****
C****
C**** M=0  14 RAYS ARE USED TO EVALUATE THE ABERRATION COEFFICIENTS FOR
C**** A POINT SOURCE OBJECT THROUGH 4'TH ORDER
C**** M=1   6 RAYS ARE USED TO EVALUATE THE ABERRATION COEFFICIENTS FOR
C**** A POINT SOURCE OBJECT THROUGH 4'TH ORDER; MIDPLANE ONLY
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 KT, LP
      character*8 L(2,50), LX(2,12)
      LOGICAL LPLT
      COMMON  /BLCK00/ LPLT
      COMMON  /BLCK 1/ XI, YI, ZI, VXI, VYI, VZI, DELP
      COMMON  /BLCK 2/ XO, YO, ZO, VXO, VYO, VZO,RTL(1000),RLL(1000)
      COMMON /BLCK 3/ XOR,  YOR,  ZOR , TH0, PH0, TL1
      DIMENSION XI(1000),YI(1000),ZI(1000),VXI(1000),VYI(1000), 
     1     VZI(1000),DELP(1000)
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
      DIMENSION CXX(12,10), IX(12), CD(6,4),   LFACT(50), C(50,10)
      DIMENSION DXX(21,10), DXY(21,10)
      DATA IX/ 1,2,5,7,11,13,19,22,29,32,35,36  /
      DATA LFACT / 1,0,1,0,2*4,2*3,4,3,2*7,2*6,2*7,2*6,3*10,3*9,
     1   2*10,2*9,3*13,12,4,7,4,7,14*0   /
      DATA  L  / 'X/TH    ','       =','T/TH    ','       =',
     1           'Y/PH    ','       =','P/PH    ','       =',
     2           'X/TH**2 ','       =','X/PH**2 ','       =',
     3           'T/TH**2 ','       =','T/PH**2 ','       =',
     4           'Y/TH*PH ','       =','P/TH*PH ','       =',
     5           'X/TH**3 ','       =','X/TH*PH*','*2     =',
     6           'T/TH**3 ','       =','T/TH*PH*','*2     =',
     7           'Y/PH**3 ','       =','Y/TH**2*','PH     =',
     8           'P/PH**3 ','       =','P/TH**2*','PH     =',
     9           'X/TH**4 ','       =','X/TH**2*','PH**2  =',
     A           'X/PH**4 ','       =','T/TH**4 ','       =',
     B           'T/TH**2*','PH**2  =','T/PH**4 ','       =',
     C           'Y/TH**3*','PH     =','Y/TH*PH*','*3     =',
     D           'P/TH**3*','PH     =','P/TH*PH*','*3     =',
     E           'X/TH**5 ','       =','X/TH**3*','PH**2  =',
     F           'X/TH*PH*','*4     =','T/TH**5 ','       =',
     G           'X/PH**2(','TRUNC.)=','X/TH*PH*','*2(TR.)=',
     H           'X/T**2 (','TRUNC.)=','X/T**3 (','TRUNC.)=',
     i            28*'        '/
      DATA  LX / 'ENERGY(M','EV)    =','XOR (CM)','       =',
     1           'YOR (CM)','       =','ZOR (CM)','       =',
     2           'TH  (MR)','       =','PHI (MR)','       =',
     3           ']XMAX](C','M)     =','2]YMAX](','CM)    =',
     4           ']X-WAIST','](CM)  =','X(X-WAIS','T)     =',
     5           'Z(X-WAIS','T)     =','LENGTH(C','M)     ='  /
C****
      MM=M
C****
      I   = JEN
      IF( I   .GT. 10 ) I   = 10
C****
C****
      XMIN = XO(1)
      XMAX = XO(1)
      YMAX = DABS(YO(1))
      DO 4 J=2,NR
      IF( XO(J) .GT. XMAX )  XMAX = XO(J)
      IF( XO(J) .LT. XMIN )  XMIN = XO(J)
      IF( DABS(YO(J) ) .GT. YMAX ) YMAX = DABS(YO(J))
    4 CONTINUE
      CXX(1,I  ) = ENERGY
      CXX(2,I  ) = XOR
      CXX(3,I  ) = YOR
      CXX(4,I  ) = ZOR
      CXX(5,I  )=TH0
      CXX(6,I  )=PH0
      CXX(7,I  ) = DABS(XMAX-XMIN)
      CXX(8,I  ) = 2.*YMAX
C****
C****   CALCULATE BEAM WIDTH AT TEN EQUALLY SPACED (5MM)
C****   DISTANCES EACH SIDE OF ZOR
C****
      DO 20 JJ=1,21
      XMIN = XO(1) + 0.00050 * VXO(1) * (JJ-11)
      XMAX = XMIN
      DO 21 J = 2, 6
      XJJ = XO(J) + 0.00050 *VXO(J) * (JJ-11)
      IF (XJJ.GT.XMAX) XMAX = XJJ
      IF (XJJ.LT.XMIN) XMIN = XJJ
   21 CONTINUE
      DXX(JJ,I) = DABS( XMAX - XMIN)
      DXY(JJ,I) = 0.
      IF (NR.LE.6) GOTO 20
      DO 22 J=7,NR
      XJJ = XO(J) + 0.00050* VXO(J) * (JJ-11)
      IF ( XJJ.GT.XMAX ) XMAX = XJJ
      IF ( XJJ.LT.XMIN ) XMIN = XJJ
   22 CONTINUE
      DXY(JJ,I) = DABS( XMAX - XMIN)
   20 CONTINUE
C****
C**** CALCULATE POSITION OF MINIMUM BEAM WIDTH
C**** WITHIN 10.0 CM OF ZOR
      XMX = 1.0D20
      DO 25  JJ=1, 101
      XMIN = XO(1) + 0.00020 * VXO(1) * (JJ-51)
      XMAX = XMIN
      DO 26  J=2,NR
      XJJ = XO(J) + 0.00020 * VXO(J) * (JJ-51)
      IF ( XJJ.GT.XMAX ) XMAX = XJJ
      IF ( XJJ.LT.XMIN ) XMIN = XJJ
   26 CONTINUE
      DXMAX = DABS( XMAX - XMIN )
      IF ( DXMAX.GE.XMX ) GO TO 25
      XMX = DXMAX
      ZMX = 0.20 * (JJ - 51)
   25 CONTINUE
      IF ( DABS( ZMX ).GT.9.9 ) ZMX = 1.0D20
      CXX( 9, I) = XMX
      CXX(10, I) = .001*TH0*ZMX + XOR
      CXX(11, I) = ZMX+ZOR
      CXX(12, I) = TL1
C****
C****
      IF( VXI(2) .EQ. 0. )  VXI(2) = 1.D-30
      IF( VXI(3) .EQ. 0. )  VXI(3) = 1.D-30
      KT = VXI(5)/VXI(3)
      DTH = VXI(3)
      TMAX = VXI(5)
      PMAX = VYI(12)
      XT=XO(2)/VXI(2)
      TT=(KT**3*(VXO(3)-VXO(4))- VXO(5)+VXO(6))/(2.* (KT**3-KT)*DTH)
      XTT = ( KT**4*(XO(3) + XO(4)) - (XO(5)+XO(6) )) /
     1   (2.*(KT**4-KT**2) *DTH*DTH)
      TTT = ( KT**4*(VXO(3)+VXO(4)) -(VXO(5)+VXO(6))) /
     1   (2.*(KT**4-KT**2) *DTH*DTH)
      XTTT  = ( KT**5 * ( XO(3) - XO(4) - 2.*XT*DTH ) -
     1 ( XO(5) - XO(6) -2.*KT*XT*DTH) ) / (2.*(KT**5 - KT**3) *DTH**3 )
      TTTT  = (-KT    * (VXO(3) -VXO(4)) + (VXO(5) -VXO(6) )  ) /
     1   (2.*(KT**3 - KT   ) *DTH**3 )
      XTTTT = ( (XO(5)+XO(6))-KT*KT*(XO(3)+XO(4) ) ) /
     1   (2.*(KT**4 - KT*KT)*DTH**4 )
      TTTTT =((VXO(5)+VXO(6))-KT*KT*(VXO(3)+VXO(4))) /
     1   (2.*(KT**4 - KT*KT)*DTH**4 )
      XTTTTT= ( XO(5) - XO(6) - 2.*KT*XT*DTH - KT**3*( XO(3) - XO(4) -
     1   2.*XT*DTH) ) / ( 2.*(KT**5 - KT**3) *DTH**5 )
      TTTTTT= 0.
C****
C****
      C( 1,I)      = XT*10.
      C( 2,I)      = TT
      C( 5,I)      = XTT*10.**4
      C( 7,I)      = TTT*10.**3
      C(11,I)      = XTTT*10.**7
      C(13,I)      = TTTT*10.**6
      C(19,I)      = XTTTT*10.**10
      C(22,I)      = TTTTT*10.**09
      C(29,I)      = XTTTTT*10.**13
      C(32,I)      = TTTTTT*10.**12
      C(35,I)      = (XTT + XTTTT*TMAX*TMAX)*10.**4
      C(36,I)      = (XTTT+XTTTTT*TMAX*TMAX)*10.**7
C****
C****
      IF( M .NE. 0 ) GO TO 1
      LP = VYI(12)/VYI(7)
      DPH = VYI(7)
      XPP   = (LP**4*XO(7) - XO(12)) /((LP**4 - LP*LP)*DPH*DPH )
      TPP   = (LP**4*VXO(7)-VXO(12)) /((LP**4 - LP*LP)*DPH*DPH )
      XPPPP = (XO(12)-LP*LP*XO(7) ) /((LP**4-LP*LP)*DPH**4)
      TPPPP =(VXO(12)-LP*LP*VXO(7)) /((LP**4-LP*LP)*DPH**4)
      XTPP  = (LP**4*( XO(8) - XO(9)) - ( XO(13) - XO(14)) - (LP**4-1.)*
     1   ( XO(3) - XO(4)) -(( XO(10) - XO(11)) - KT*( XO(8) - XO(9) ) -
     2   ( XO(5) - XO(6) ) + KT*( XO(3) - XO(4) ) ) *
     3   ( ( LP**4 - LP*LP) / (KT**3-KT) ))/(2.*(LP**4-LP*LP)*
     4   DTH*DPH*DPH )
      TTPP  = 0.
      XTTPP = ( ( XO(8)+XO(9) ) - ( XO(3)+XO(4) ) - 2.*XO(7)) /
     1   (2.*DTH*DTH*DPH*DPH)
      TTTPP = ( (VXO(8)+VXO(9)) - (VXO(3)+VXO(4)) -2.*VXO(7)) /
     1   (2.*DTH*DTH*DPH*DPH)
      YP    = ( LP**3 * YO(7) - YO(12) ) / ( (LP**3 - LP)*DPH )
      PP    = ( LP**3 *VYO(7) -VYO(12) ) / ( (LP**3 - LP)*DPH )
      YPPP  = (YO(12) - LP*YO(7)) /((LP**3-LP)*DPH**3 )
      PPPP  =(VYO(12) -LP*VYO(7)) /((LP**3-LP)*DPH**3 )
      YTTP  = ( YO(8) + YO(9) - 2.*YO(7) ) / (2.*DTH*DTH*DPH )
      PTTP  = (VYO(8) +VYO(9) - 2.*VYO(7)) / (2.*DTH*DTH*DPH )
      YTPPP = ( YO(13) - LP*YO(8) - YO(12) + LP*YO(7) ) /
     1   ((LP**3 - LP)*DTH*DPH**3 )
      PTPPP = (VYO(13) - LP*VYO(8)-VYO(12) + LP*VYO(7)) /
     1   ((LP**3 - LP)*DTH*DPH**3 )
      YTTTP = ( YO(10) - YO(11) -KT*(YO(8)-YO(9) ) ) /
     1   (2.*(KT**3-KT) * DTH**3*DPH )
      PTTTP = (VYO(10) -VYO(11) -KT*(VYO(8)-VYO(9))) /
     1   (2.*(KT**3-KT) * DTH**3*DPH )
      YTP   = ( (YO(10)-YO(11) -KT**3*(YO(8)-YO(9) ) ) /(2.*(KT-KT**3))-
     1   YTPPP*DTH*DPH**3 ) /(DTH*DPH)
      PTP   = ((VYO(10)-VYO(11)-KT**3*(VYO(8)-VYO(9))) /(2.*(KT-KT**3))-
     1   PTPPP*DTH*DPH**3 ) /(DTH*DPH)
      XTTTPP= ( XO(10) - XO(11) - KT*( XO(8) - XO(9)) - ( XO(5) - XO(6))
     1   +KT*( XO(3) - XO(4) ) ) / (2.*(KT**3-KT) * DTH**3*DPH*DPH )
      TTTTPP= 0.
      XTPPPP= ( XO(13) - XO(14) - LP*LP*( XO(8) - XO(9)) + (LP*LP-1.) *
     1   ( XO(3) - XO(4) ) ) / (2.*(LP**4-LP*LP) * DTH*DPH**4 )
      TTPPPP= 0.
      C( 3,I)      = YP*10.
      C( 4,I)      = PP
      C( 6,I)      = XPP*10.**4
      C( 8,I)      = TPP*10.**3
      C( 9,I)      = YTP*10.**4
      C(10,I)      = PTP*10.**3
      C(12,I)      = XTPP*10.**7
      C(14,I)      = TTPP*10.**6
      C(15,I)      = YPPP*10.**7
      C(16,I)      = YTTP*10.**7
      C(17,I)      = PPPP*10.**6
      C(18,I)      = PTTP*10.**6
      C(20,I)      = XTTPP*10.**10
      C(21,I)      = XPPPP*10.**10
      C(23,I)      = TTTPP*10.**09
      C(24,I)      = TPPPP*10.**09
      C(25,I)      = YTTTP*10.**10
      C(26,I)      = YTPPP*10.**10
      C(27,I)      = PTTTP*10.**09
      C(28,I)      = PTPPP*10.**09
      C(30,I)      = XTTTPP*10.**13
      C(31,I)      = XTPPPP*10.**13
      C(33,I)      = (XPP + XPPPP*PMAX*PMAX)*10.**4
      C(34,I)      = (XTPP + XTTTPP*TMAX*TMAX +XTPPPP*PMAX*PMAX)*10.**7
C****
C****
   13 FORMAT( 2I5 )
   14 FORMAT(   )
   15 FORMAT( //  , 8( 15X, 2A8,  F9.4 /  ) /,3( 15X, 2A8, F8.3/))
   16 FORMAT(    15X, 2A8, 1PE12.3, 0PF15.4   )
c      write (6, 15) ( ( LX(J,K),J=1,2),  CXX(K,I), K=1,12)
      DO 2 JJ=1,36
      COEF = C(JJ,I)/ 10.**LFACT(JJ)
c      IF((JJ.EQ.5).OR.(JJ.EQ.11).OR.(JJ.EQ.19).OR.(JJ.EQ.29))
c     1 write(6,14)
c****
    2 CONTINUE
c    2 write (6, 16) (L(J,JJ), J=1,2), COEF, C(JJ,I)
      GO TO 23
C****
C****
    1 CONTINUE
c      write (6, 15) (( LX(J,K),J=1,2),  CXX(K,I), K=1,12)
      DO 3 JJ=1,12
      K = IX(JJ)
      COEF = C(K,I)/10.**LFACT(K)
    3 CONTINUE
c    3 write (6, 16) ( L(J,K),J=1,2), COEF, C(K,I)
C****
C****   PRINT OUT BEAM WIDTH
C****
   23 CONTINUE
C**     write (6, 29)
C**     DO 24 JJ=1, 21
C**     DZ = 0.50 * (JJ-11)
C**     write (6, 30) DZ, DXX(JJ,I), DXY(JJ,I)
C**24   CONTINUE
C**29   FORMAT ('1', 3X, 'IMAGE SIZE ]XMAX](CM)', //2X, 'DZ (CM)',
C**  1  2X, '  1-6  ', 2X, '  1-NR')
C**30   FORMAT (F8.2, 2F9.3)
      RETURN
C****
C****
      ENTRY MPRNT( NEN )
C****
      IF( LPLT) WRITE(2,13) NEN, MM
   18 FORMAT(   4X, 2A8, 10F11.3      )
      IF( NEN .GT. 10 )  NEN = 10
c      write (6, 14)
      DO 8 K=1,8
      IF( LPLT ) WRITE(2,18)(LX(J,K),J=1,2),(CXX(K,I),I=1,NEN)
    8 CONTINUE
c    8 write (6, 18)   ( LX(J,K),J=1,2),(CXX(K,I),I=1,NEN)
c      write (6, 14 )
C****
      IF(MM .NE. 0 )  GO TO 5
      DO 7 K=1,36
c      IF( (K.EQ.5).OR.(K.EQ.11).OR.(K.EQ.19).OR.(K.EQ.29))
c     1 write(6,14)
      IF( LPLT ) WRITE(2,18) (L(J,K),J=1,2),(C(K,I),I=1,NEN )
    7 CONTINUE
c    7 write (6, 18) ( L(J,K),J=1,2),(C(K,I),I=1,NEN )
      GO TO 19
    5 DO 6 JJ=1,12
      K = IX(JJ)
      IF( LPLT ) WRITE(2,18) ( L(J,K),J=1,2), ( C(K,I), I=1,NEN)
    6 CONTINUE
c    6 write (6, 18) ( L(J,K),J=1,2),(C(K,I), I=1,NEN )
C****
C**** CHROMATIC ABERRATION COEFFICIENTS
C**** CALCULATED ONLY FOR CASE OF NEN= 5 ENERGIES
C****
   19 CONTINUE
      IF( NEN .NE. 5 ) RETURN
      DEL = CXX(1,4)/CXX(1,3) - 1.
      DO 9 I=1,6
      IF( I .EQ. 1 ) K=2
      IF( I .EQ. 2 ) GO TO 10
      IF( I .EQ. 3 ) K=5
      IF( I .EQ. 4 ) K=11
      IF( I .EQ. 5 ) K=19
      IF( I .EQ. 6 ) K=29
      IF( I .GT. 2 ) GO TO 11
      X1 =(CXX(K,1) - CXX(K,3))/100.
      X2 =(CXX(K,2) - CXX(K,3))/100.
      X4 =(CXX(K,4) - CXX(K,3))/100.
      X5 =(CXX(K,5) - CXX(K,3))/100.
      GO TO 12
   11 X1 = C(K,1) - C(K,3)
      X2 = C(K,2) - C(K,3)
      X4 = C(K,4) - C(K,3)
      X5 = C(K,5) - C(K,3)
   12 CD(I,1) = (8. *(X4-X2) - (X5-X1) )/(12.  *DEL)
      CD(I,2) = (16.* (X4+X2) - (X5+X1) )/(24.  *DEL*DEL)
      CD(I,3) = ( (X5-X1) - 2.*(X4-X2) )/(12.  *DEL**3)
      CD(I,4) = ( (X5+X1) - 4.*(X4+X2) )/(24.  *DEL**4)
      GO TO 9
   10 Z1 =(CXX(4,1) - CXX(4,3))/100.
      Z2 =(CXX(4,2) - CXX(4,3))/100.
      Z4 =(CXX(4,4) - CXX(4,3))/100.
      Z5 =(CXX(4,5) - CXX(4,3))/100.
      TPSI = (8.* (Z4-Z2) - (Z5-Z1) ) / (8.* (X4-X2) - (X5-X1) )
      PSI = 57.29578D0 * DATAN(TPSI)
      DZ1 = Z1 - X1*TPSI
      DZ2 = Z2 - X2*TPSI
      DZ4 = Z4 - X4*TPSI
      DZ5 = Z5 - X5*TPSI
      CD(I,1) = -C(2,3)*( 8.*(DZ4-DZ2) - (DZ5-DZ1) )/(12.  *DEL)
      CD(I,2) = -C(2,3)*( 16.*(DZ4+DZ2) - (DZ5+DZ1) )/(24.  *DEL*DEL)
      CD(I,3) = -C(2,3)*( (DZ5-DZ1) - 2.*(DZ4-DZ2) )/(12.  *DEL**3)
      CD(I,4) = -C(2,3)*( (DZ5+DZ1) - 4.*(DZ4+DZ2) )/(24.  *DEL**4)
    9 CONTINUE
c      write (6, 14)
c      write (6, 17) PSI, (I,I=1,4), ( (CD(K,I),I=1,4), K=1,6 )
      IF( LPLT ) WRITE(2,17) PSI, (I,I=1,4), ( ( CD(K,I),I=1,4), K=1,6 )
   17 FORMAT(4X,'PSI            =', F11.3,/4X,'N              =',4(I7,
     1  4X),/4X,'X/D**N         =',4F11.3,/4X,'X/T*D**N       =',4F11.3,
     2      /4X,'X/T**2*D**N    =',4X,1P4E11.3,
     3      /4X,'X/T**3*D**N    =',4X,1P4E11.3,
     4      /4X,'X/T**4*D**N    =',4X,1P4E11.3,
     5      /4X,'X/T**5*D**N    =',4X,1P4E11.3             )
      RETURN
      END

      SUBROUTINE MULT   ( NO, NP, T, TP ,NUM )
C****
C****
C**** MULTIPOLE     RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF, K, L
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLK100/  W, L, D, DG, S, BF, BT
      COMMON  /BLK101/  C0, C1, C2, C3, C4, C5, C6, C7, C8
      DIMENSION DATA(  75,200 ), ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
      EXTERNAL BMULT
C**** DATA  C/ 3.D10/
C****
      LF   = DATA(  1,NO )
      DG   = DATA(  2,NO )
      A    = DATA( 10,NO )
      B    = DATA( 11,NO )
      L    = DATA( 12,NO )
      W    = DATA( 13,NO )
      D    = DATA( 14,NO )
      BF   = DATA( 15,NO )
      Z1   = DATA( 16,NO )
      Z2   = DATA( 17,NO )
      C0   = DATA( 20,NO )
      C1   = DATA( 21,NO )
      C2   = DATA( 22,NO )
      C3   = DATA( 23,NO )
      C4   = DATA( 24,NO )
      C5   = DATA( 25,NO )
      C6   = DATA( 26,NO )
      C7   = DATA( 27,NO )
      C8   = DATA( 28,NO )
      DTF = LF/VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S = 0.
C****
      IF( NP  .GT. 100 ) GO TO 5
c      write (6, 100) ITITLE(NO)
  100 FORMAT(  ' MULTIPOLE  ****  ', A4,'  *************************'/)
c      write (6, 101)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HBX, 8X, 4HY CM , 7X, 2HBY,
     1   8X, 4HZ CM, 7X, 2HBZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HB             )
      CALL PRNT2 ( T,S,XA   ,YA   ,ZA   ,BX,BY,BZ,BT,VXA  ,VYA  ,VZA   )
c      write (6, 103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO CENTERED AXIS SYSTEM ' )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO VFB COORD.
C****
    5 TC(1) =  XA
      TC(2) = YA
      TC(3) = ZA - (A+L/2.)
      TC(4) =  VXA
      TC(5) =  VYA
      TC(6) =  VZA
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FIRST FRINGE FIELD
C****
      TDT = ( Z1 - TC(3)  ) /DABS( TC(6) )
C****
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
c      IF( NP  .LE. 100) write (6, 104)
  104 FORMAT( 24H0MULTIPOLE FIELD REGION  )
      CALL FNMIRK( 6, T, DTF ,TC, DTC, DS, ES, BMULT, 0    )
      NSTEP = 0
    6 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF ,TC, DTC, DS, ES, BMULT, 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( Z2  .LE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF  =-( TC(3) - Z2  ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF ,TC, DTC, DS, ES,BMULT,  0    )
      CALL FNMIRK( 6, T,XDTF ,TC, DTC, DS, ES,BMULT,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
  105 FORMAT( 10H   NSTEPS= ,I5 )
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      TC(3) = TC(3) - (B+L/2.)
c      IF( NP  .LE. 100) write (6, 109)
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
      TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF(NP.LE.100) write (6,115) TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
C****
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF(NP.LE.100) write (6,111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=,F10.4, 3H MR  /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y=,F10.2, 3H CM  /        )
      RETURN
   99 CALL PRNT4(NO, IN)
      RETURN
      END


      SUBROUTINE MULTPL ( NO, NP, T, TP ,NUM )
C****
C****
C**** QUADRUPOLE    RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF1, LF2, LU1, K, L
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK 7/ NCODE
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK50/  D,BGRAD, S, BT
      COMMON  /BLCK51/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK52/  IN
      DIMENSION DATA(  75,200 ), ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
      EXTERNAL  BFLD
C**** DATA  C/ 3.D10/
C****
      LF1  = DATA(  1,NO )
      LU1  = DATA(  2,NO )
      LF2  = DATA(  3,NO )
      A    = DATA( 10,NO )
      B    = DATA( 11,NO )
      L    = DATA( 12,NO )
      RAD  = DATA( 13,NO )
      BF   = DATA( 14,NO )
      Z11  = DATA( 15,NO )
      Z12  = DATA( 16,NO )
      Z21  = DATA( 17,NO )
      Z22  = DATA( 18,NO )
      DTF1= LF1/ VEL
      DTF2= LF2/ VEL
      DTU = LU1/ VEL
      D = 2. * RAD
      BGRAD = (-1)**NCODE * BF/RAD**NCODE
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S = 0.
C****
      IF( NP  .GT. 100 ) GO TO 5
  201 FORMAT(  ' QUADRUPOLE  ****  ', A4, '  ***********************'/)
  202 FORMAT(  ' HEXAPOLE    ****  ', A4, '  ***********************'/)
  203 FORMAT(  ' OCTAPOLE    ****  ', A4, '  ***********************'/)
  204 FORMAT(  ' DECAPOLE    ****  ', A4, '  ***********************'/)
      GO TO ( 21, 22, 23, 24 ) , NCODE
   21 CONTINUE
c   21 write (6, 201) ITITLE(NO)
      GO TO 25
   22 CONTINUE
c   22 write (6, 202) ITITLE(NO)
      GO TO 25
   23 CONTINUE
c   23 write (6, 203) ITITLE(NO)
      GO TO 25
   24 CONTINUE
c   24 write (6, 204) ITITLE(NO)
   25 CONTINUE
c   25 write (6, 101)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HBX, 8X, 4HY CM , 7X, 2HBY,
     1   8X, 4HZ CM, 7X, 2HBZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HB             )
      CALL PRNT2 ( T,S,XA   ,YA   ,ZA   ,BX,BY,BZ,BT,VXA  ,VYA  ,VZA   )
c      write (6, 103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO B AXIS SYSTEM '       )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO VFB COORD.
C****
    5 TC(1) = -XA
      TC(2) = YA
      TC(3) = A - ZA
      TC(4) = -VXA
      TC(5) = VYA
      TC(6) = -VZA
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FIRST FRINGE FIELD
C****
      TDT = ( TC(3) - Z11 ) /DABS( TC(6) )
C****
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** IN DESIGNATES FIELD REGIONS FOR QUADRUPOLE
C****
      IN = 1
      C0   = DATA( 19,NO )
      C1   = DATA( 20,NO )
      C2   = DATA( 21,NO )
      C3   = DATA( 22,NO )
      C4   = DATA( 23,NO )
      C5   = DATA( 24,NO )
c      IF( NP  .LE. 100) write (6, 104)
  104 FORMAT( 22H0FRINGING FIELD REGION    )
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BFLD , 0    )
      NSTEP = 0
    6 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BFLD , 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( Z12 .GE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 =-( Z12 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BFLD ,  0    )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BFLD ,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
  105 FORMAT( 10H   NSTEPS=  ,I5 )
C***
C***  UNIFORM FIELD REGION
C**** TRANSFORM TO SECOND VFB COORD SYSTEM
C***
      BGRAD = (-1)**NCODE *  BGRAD
      TC(1) = -TC(1)
      TC(3) = -TC(3) - L
      TC(4) = -TC(4)
      TC(6) = -TC(6)
C****
C****
C**** UNIFORM FIELD INTEGRATION REGION
C****
C****
      IN = 2
c      IF( NP  .LE. 100) write (6, 106)
  106 FORMAT(   '0UNIFORM FIELD REGION IN C AXIS SYSTEM '  )
      IF( TC(3)  .LT.  Z21 ) GO TO 15
C****
C**** THIS SECTION CORRECTS FOR MAGNETS WHOSE FRINGING FIELDS INTERSECT
C****
c      IF( NP  .LE. 100) write (6, 102)
  102 FORMAT( / '   INTEGRATE BACKWARDS    '  )
      CALL FNMIRK( 6, T,-DTU ,TC, DTC, DS, ES, BFLD,  0    )
      NSTEP = 0
   16 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 17  I =1, NP
      CALL FNMIRK( 6, T,-DTU, TC, DTC, DS, ES, BFLD,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .LE.  Z21 )  GO TO 18
   17 CONTINUE
      GO TO 16
   18 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BFLD,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BFLD,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
c      IF( NP  .LE. 100) write (6, 107)
  107 FORMAT( / )
C%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
C%%%%
      IF(TC(3).GE.Z21) GO TO 11
      GO TO 9
C%%%% GO TO 19    ...   REMOVED IN FAVOR OF ABOVE TWO STATEMENTS  T.M.C.
C%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
C****
C****
   15 CONTINUE
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, BFLD , 0    )
      NSTEP = 0
    9 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 10  I =1, NP
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, BFLD , 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
   19 CONTINUE
      IF( TC(3)  .GE.  Z21 )  GO TO 11
   10 CONTINUE
      GO TO 9
   11 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BFLD ,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BFLD ,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C***
C***
C**** SETUP FOR SECOND FRINGE FIELD AND INTEGRATION
C****
C****
      C0   = DATA( 25,NO )
      C1   = DATA( 26,NO )
      C2   = DATA( 27,NO )
      C3   = DATA( 28,NO )
      C4   = DATA( 29,NO )
      C5   = DATA( 30,NO )
      IN = 3
c      IF( NP  .LE. 100) write (6, 104)
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BFLD , 0    )
      NSTEP = 0
   12 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 13  I =1, NP
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BFLD , 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3) .GE. Z22 )  GO TO 14
   13 CONTINUE
      GO TO 12
   14 CONTINUE
      XDTF2 = ( Z22 - TC(3) ) / TC(6)
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BFLD , 0    )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BFLD , 1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      TC(3) = TC(3) - B
c      IF( NP  .LE. 100) write (6, 109)
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
      TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF(NP.LE.100) write (6, 115)TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
C****
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF( NP  .LE. 100) write (6, 111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=,F10.4, 3H MR  /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y=,F10.2, 3H CM  /        )
      RETURN
   99 CALL PRNT4 (NO, IN)
      RETURN
      END


      SUBROUTINE BDIP
C****
C****
C**** MTYP=1  :    UNIFORM FIELD STANDARD APPROXIMATION
C**** MTYP=2  :    UNIFORM FIELD MODIFIED ITERATIVE PROCEDURE
C**** MTYP=3  :    NONUNIFORM FIELD STANDARD APPROXIMATION
C**** MTYP=4  :    NONUNIFORM FIELD  B=BF/(1+N*DR/R)
C**** MTYP=5  :    UNIFORM FIELD, CIRCULAR POLE OPTION
C**** MTYP=6  :    PRETZEL MAGNET
C****
C**** THE RELATIONSHIP BETWEEN B0, ......... B12 AND B(I,J) RELATIVE TO
C**** AXES (Z,X) IS GIVEN BY
C****
C****
C****
C**** B0  = B( 0, 0 )
C**** B1  = B( 1, 0 )
C**** B2  = B( 2, 0 )
C**** B3  = B( 1, 1 )
C**** B4  = B( 1,-1 )
C**** B5  = B( 0, 1 )
C**** B6  = B( 0, 2 )
C**** B7  = B( 0,-1 )
C**** B8  = B( 0,-2 )
C**** B9  = B(-1, 0 )
C**** B10 = B(-2, 0 )
C**** B11 = B(-1, 1 )
C**** B12 = B(-1,-1 )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  NDX, K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
      DIMENSION XX(11), ZZ(11), DD(11)
      DATA PI2   / 1.570796325D0       /
      DATA PI4   / .7853981625D0       /
      DATA RT2   /  1.41421356D0       /
C****
C****
      GO TO ( 10,10,6,6,10,21 )     ,MTYP
      CALL EXIT
      RETURN
    6 CALL NDIP
      RETURN
   21 CALL BPRETZ
      RETURN
C****
C**** MTYP = 1 , 2, 5
C**** UNIFORM FIELD MAGNETS
C****
   10 CONTINUE
      GO TO( 2, 1, 2, 4 ) , IN
    7 CONTINUE
c    7 write(6,8) IN
    8 FORMAT(  35H0 ERROR -GO TO -  IN BFUN   IN=        ,I5  )
    1 BX = 0.
      BY = BF
      BZ = 0.
      BT = BF
      RETURN
    2 X = TC(1)
      Y = TC(2)
      Z = TC(3)
      IF( MTYP .NE. 2 ) GO TO 9
C****
C**** MTYP=2  :    UNIFORM FIELD MODIFIED ITERATIVE PROCEDURE
C****
      XP = X
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3 * XP
      ZP = -(S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
     1       S7*XP4*XP3 + S8*XP4*XP4 )
      AZ = (Z-ZP)/5.D0
      DO 11 I=1,11
      XP    = X + AZ*(I-6)
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3*XP
      ZP = -(S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
     1       S7*XP4*XP3 + S8*XP4*XP4 )
      XXP = X-XP
      ZZP = Z-ZP
      XX(I) = XP
      ZZ(I) = ZP
      DD(I) = ( XXP*XXP + ZZP*ZZP )
   11 CONTINUE
C****
C**** SEARCH FOR SHORTEST OF THE 11 DISTANCES
C****
      XP = XX(1)
      ZP = ZZ(1)
      DP = DD(1)
      DO 12 I=2,11
      IF( DD(I) .GE. DP ) GO TO 12
      XP = XX(I)
      ZP = ZZ(I)
      DP = DD(I)
   12 CONTINUE
C****
C****  DIVIDE INTERVAL AND REPEAT FOR MORE EXACT
C****  SHORTEST DISTANCE.
C****
      AZ = AZ/5.D0
      X1 = XP
      DO 13 I=1,11
      XP    = X1+ AZ*(I-6)
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3*XP
      ZP = -(S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
     1       S7*XP4*XP3 + S8*XP4*XP4 )
      XXP = X-XP
      ZZP = Z-ZP
      XX(I) = XP
      ZZ(I) = ZP
      DD(I) = ( XXP*XXP + ZZP*ZZP )
   13 CONTINUE
C****
C**** SEARCH FOR SHORTEST OF THE 11 DISTANCES
C****
      XP = XX(1)
      ZP = ZZ(1)
      DP = DD(1)
      DO 15 I=2,11
      IF( DD(I) .GE. DP ) GO TO 15
      XP = XX(I)
      ZP = ZZ(I)
      DP = DD(I)
   15 CONTINUE
C****
C**** ITERATION LOOP FOR MORE EXACT SHORTEST DISTANCE
C****
C*    ZSIGN = Z-ZP
C*    XP2 = XP*XP
C*    XP3 = XP2*XP
C*    XP4 = XP3*XP
C*    DO 13 I=1,3
C****
C**** SLOPE OF CURVE AT XP, ZP
C****
C*    DZDXC = -(2.*S2*XP + 3.*S3*XP2+ 4.*S4*XP3 + 5.*S5*XP4 +
C*   1   6.*S6*XP4*XP + 7.*S7*XP4*XP2 + 8.*S8*XP4*XP3 )
C****
C**** NEXT APPROXIMATION TO CLOSEST POINT IS
C****
C*    XP = ( DZDXC*(Z-ZP)  +  DZDXC*DZDXC*XP + X ) / (1.+DZDXC*DZDXC)
C*    IF( I  .EQ.  1 )  XP = (3.*XP +  X ) / 4.
C*    XP2 = XP*XP
C*    XP3 = XP2*XP
C*    XP4 = XP3*XP
C*    ZP = -( S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
C*   1   S7*XP4*XP3 + S8*XP4*XP4 )
C* 13 CONTINUE
C*    XXP = X-XP
C*    ZZP = Z-ZP
C****
C****
      ZSIGN = Z-ZP
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3*XP
      S = DSIGN( 1.D0,ZSIGN) * DSQRT(DP)                 / D + DELS
      SCON = S
C****
C**** TRIM CORRECTION FOR EFFECTIVE EDGE CURVATURE
C****
      DZDXC = -(2.*S2*XP + 3.*S3*XP2+ 4.*S4*XP3 + 5.*S5*XP4 +
     1   6.*S6*XP4*XP + 7.*S7*XP4*XP2 + 8.*S8*XP4*XP3 )
      DZDXC2= -(2.*S2+6.*S3*XP +12.*S4*XP2 +20.*S5*XP3 +30.*S6*XP4 +
     1         42.*S7*XP4*XP +56.*S8*XP4*XP2 )
      RCR = DZDXC2 / DSQRT( 1.D0 + DZDXC*DZDXC ) **3
      S = S + SCOR*D*RCR
      S0 = S
      CALL BDPP( B0, Z, X, Y )
      IF( Y .NE. 0. ) GO TO 14
      BX = 0.
      BY = B0
      BZ = 0.
      BT = B0
      RETURN
   14 GD = DG/D
      DELTA = DATAN(DZDXC)
      SCON = (1.D0 + SCON*D*RCR) *GD*DG*RCR/2.D0
      DCS = DCOS( DELTA         )
      S = S0-  SCON*( 1.D0 - DCS*DCS ) +      GD*DCS
      CALL BDPP( B1 , Z, X, Y )
      S =S0-4.*SCON*( 1.D0 - DCS*DCS ) +   2.*GD*DCS
      CALL BDPP( B2 , Z, X, Y )
      S = S0-  SCON*( 1.D0 - DCS*DCS ) -      GD*DCS
      CALL BDPP( B9 , Z, X, Y )
      S =S0-4.*SCON*( 1.D0 - DCS*DCS ) -   2.*GD*DCS
      CALL BDPP( B10, Z, X, Y )
      DCS = DCOS( DELTA + PI4   )
      S =S0-2.*SCON*( 1.D0 - DCS*DCS ) +  RT2*GD*DCS
      CALL BDPP( B3 , Z, X, Y )
      S =S0-2.*SCON*( 1.D0 - DCS*DCS ) -  RT2*GD*DCS
      CALL BDPP( B12, Z, X, Y )
      DCS = DCOS( DELTA - PI4   )
      S =S0-2.*SCON*( 1.D0 - DCS*DCS ) +  RT2*GD*DCS
      CALL BDPP( B4 , Z, X, Y )
      S =S0-2.*SCON*( 1.D0 - DCS*DCS ) -  RT2*GD*DCS
      CALL BDPP( B11, Z, X, Y )
      DCS = DCOS( DELTA + PI2   )
      S = S0-  SCON*( 1.D0 - DCS*DCS ) +      GD*DCS
      CALL BDPP( B5 , Z, X, Y )
      S =S0-4.*SCON*( 1.D0 - DCS*DCS ) +   2.*GD*DCS
      CALL BDPP( B6 , Z, X, Y )
      S = S0-  SCON*( 1.D0 - DCS*DCS ) -      GD*DCS
      CALL BDPP( B7 , Z, X, Y )
      S =S0-4.*SCON*( 1.D0 - DCS*DCS ) -   2.*GD*DCS
      CALL BDPP( B8 , Z, X, Y )
      GO TO 5
    9 CALL BDPP ( B0, Z, X, Y )
      S0 = S
      IF( Y .NE. 0. )   GO TO 3
      BX = 0.
      BY = B0
      BZ = 0.
      BT = B0
      RETURN
    3 CALL BDPP ( B1 , Z + DG, X , Y )
      CALL BDPP ( B2 , Z + 2.*DG, X , Y )
      CALL BDPP ( B3 , Z + DG, X + DG , Y )
      CALL BDPP ( B4 , Z + DG, X - DG , Y )
      CALL BDPP ( B5 , Z , X + DG , Y )
      CALL BDPP ( B6 , Z , X + 2.*DG , Y )
      CALL BDPP ( B7 , Z , X - DG , Y )
      CALL BDPP ( B8 , Z , X - 2.*DG , Y )
      CALL BDPP ( B9 , Z - DG, X , Y )
      CALL BDPP ( B10, Z - 2.*DG, X , Y )
      CALL BDPP ( B11, Z - DG, X + DG , Y )
      CALL BDPP ( B12, Z - DG, X - DG , Y )
    5 CONTINUE
      S = S0
      YG1 = Y/DG
      YG2 = YG1**2
      YG3 = YG1**3
      YG4 = YG1**4
      BX = YG1 * ( (B5-B7)*2./3. - (B6-B8)/12. )  +
     1     YG3*( (B5-B7)/6. - (B6-B8)/12. -
     2     (B3 + B11 - B4 - B12 - 2.*B5 + 2.*B7 ) / 12. )
      BY = B0 - YG2*( ( B1 + B9 + B5 + B7 - 4.*B0 ) *2./3. -
     1     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. ) +
     2     YG4* (-( B1 + B9 + B5 + B7 - 4.*B0 ) / 6. +
     3     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. +
     4     ( B3 + B11 + B4 + B12 - 2.*B1 - 2.*B9 -
     5     2.*B5 - 2.*B7 + 4.*B0 ) / 12. )
      BZ = YG1*( (B1 - B9 ) *2./3. - ( B2 - B10 ) /12. ) +
     1     YG3*( ( B1 - B9 ) / 6. - ( B2 - B10 ) / 12. -
     2     ( B3 + B4 - B11 - B12 - 2.*B1 + 2.*B9 ) / 12.  )
      BT = DSQRT(BX*BX + BY*BY + BZ*BZ)
      RETURN
    4 BX = 0.
      BY = BR
      BZ = 0.
      BT = BR
      RETURN
      END


      SUBROUTINE  BDPP ( BFLD, Z, X, Y )
C****
C****
C****
C**** MTYP=1  :    UNIFORM FIELD STANDARD APPROXIMATION
C**** MTYP=2  :    UNIFORM FIELD MODIFIED ITERATIVE PROCEDURE
C****              MORE ACCURATE 3'RD AND HIGHER ORDER CURVATURES
C**** MTYP=5  :    UNIFORM FIELD, CIRCULAR POLE OPTION
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  NDX, K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
C****
      GO TO (10,13,6,6,11 ) ,MTYP
    6 CALL EXIT
      RETURN
C****
C**** MTYP=1  :    UNIFORM FIELD STANDARD APPROXIMATION
C****
   10 X2=X*X
      X3=X*X2
      X4=X*X3
      S = (    Z   +S2*X2 + S3*X3 + S4*X4 + S5*X*X4 + S6*X2*X4 +
     1   S7*X3*X4 + S8*X4*X4  ) / D + DELS
      GO TO 13
C****
C**** MTYP=5  :    UNIFORM FIELD, CIRCULAR POLE OPTION
C****
   11 IF( DABS(RCA)  .GE. 1.D-08  ) GO TO 12
      S = Z/D + DELS
      GO TO 13
   12 A = 1./RCA
      S = ( DSIGN(1.D0,A) * DSQRT( (Z+A)**2 + X*X ) - A ) / D + DELS
   13 CS=C0+S*(C1+S*(C2+S*(C3+S*(C4+S*C5))))
      IF( DABS(CS)  .GT.  70.  )  CS =DSIGN( 70.D0 ,CS  )
      E=DEXP(CS)
      P0 = 1.0 + E
      DB=BF-BR
      BFLD=BR + DB/P0
C****
C**** write (6, 100) X, Y, Z,  DR, S, BFLD
C*100 FORMAT( 1P6D15.4 )
C****
      RETURN
      END


      SUBROUTINE BEFN (F,Z,X,Y,DR,IBEX)
C****
C****
C****    CALCULATES S, THEN DETERMINES B (OR E) FIELD.
C****
C****
C****
C**** IBEX = 0   MAGNETIC FIELD COMPONENTS
C****      = 1   ELECTRIC FIELD COMPONENTS
C****
      IMPLICIT  REAL*8 (A-H,O-Z)
      REAL*8 NDX
      COMMON /BLCK71/  CB0,CB1,CB2,CB3,CB4,CB5
      COMMON /BLCK72/  CE0,CE1,CE2,CE3,CE4,CE5
      COMMON /BLCK73/  IN,NFLAG
      COMMON /BLCK74/  BF,EF,S,DG
      COMMON /BLCK75/  BC2,BC4,EC2,EC4
      COMMON /BLCK76/  DB,DE,WB,WE
      COMMON /BLCK77/  RB,NDX
C****
      IF (IBEX .NE. 0 ) GO TO 10
C****
C**** MAGNETIC FIELD COMPONENTS
C****
      F1 = BF
      D = DB
      C02 = BC2
      C04 = BC4
      W2 = WB*WB
      C0 = CB0
      C1 = CB1
      C2 = CB2
      C3 = CB3
      C4 = CB4
      C5 = CB5
      GO TO 20
C****
C**** ELECTRIC FIELD COMPONENTS
C****
   10 F1 = EF
      IF( IN .EQ. 1 ) F1 = -EF
      D = DE
      C02 = EC2
      C04 = EC4
      W2 = WE*WE
      C0 = CE0
      C1 = CE1
      C2 = CE2
      C3 = CE3
      C4 = CE4
      C5 = CE5
   20 ZD1 = Z/D
      ZD2 = C02*ZD1*X*X/W2
      W4 = W2*W2
      ZD3 = C04*(X**4)/W4
      S = ZD1+ZD2+ZD3
      CS = C0+S*(C1+S*(C2+S*(C3+S*(C4+S*C5))))
      IF ( DABS(CS) .GT. 70. ) CS = DSIGN ( 70.D0,CS )
      E = DEXP(CS)
      P0 = 1.0+E
      F = F1/P0
      IF( IBEX  .EQ.  1) RETURN
      IF( NFLAG .EQ.  1) F=F*(1.D0 - (F/F1)*NDX*DR/RB)
      RETURN
      END


      SUBROUTINE BEVC
C****
C****  CALCULATES B AND E FIELDS
C****
C****
C****
C**** NFLAG = 0      UNIFORM FIELD MAGNETIC DIPOLE
C****       = 1  NON-UNIFORM FIELD MAGNETIC DIPOLE
      IMPLICIT     REAL*8 (A-H,O-Z)
      REAL*8 K,NDX
      COMMON /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON /BLCK11/  EX, EY, EZ, QMC, IVEC
      COMMON /BLCK71/  CB0,CB1,CB2,CB3,CB4,CB5
      COMMON /BLCK72/  CE0,CE1,CE2,CE3,CE4,CE5
      COMMON /BLCK73/  IN,NFLAG
      COMMON /BLCK74/  BF,EF,S,DG
      COMMON /BLCK77/  RB,NDX
      DIMENSION TC(6),DTC(6),BEF(3)
C****
      GO TO (2,1,2) , IN
c      write (6,  100) IN
  100 FORMAT (  35H0 ERROR -GO TO -  IN BFUN   IN=     ,I5 )
C****
C**** UNIFORM FIELD REGION
C****
    1 BX = 0.
      BY = BF
      BZ = 0.
      EX = EF
      EY = 0.
      EZ = 0.
      IF( NFLAG .EQ. 0 ) RETURN
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      DR =X
      RP = X+RB
      IF( RP .LE. 0. ) RP = 1.D-20
      DRR1 = DR/RB
      IF( Y .NE. 0. )  GO TO 14
C****
C**** MID-PLANE UNIFORM FIELD REGION
C****
      BY = BF* ( 1. - NDX*DRR1 )
      RETURN
C****
C**** NON MID-PLANE UNIFORM FIELD REGION
C****
   14 YR1 = Y/RB
      YR2 = YR1*YR1
      YR3 = YR2*YR1
      YR4 = YR3*YR1
      RR1 = RB/RP
      RR2 = RR1*RR1
      RR3 = RR2*RR1
      BX  = BF*( -NDX*YR1 - (NDX*RR2 )*YR3/6. )
      BY  = BF* ( 1.-NDX*DRR1+.5*YR2*NDX*RR1 - YR4*NDX*RR3/24. )
      RETURN
C****
C**** FRINGE FIELD REGIONS:  FIND B AND E FIELDS
C****
    2 X = TC(1)
      Y = TC(2)
      Z = TC(3)
      IF ( Y .EQ. 0. ) GO TO 3
C****
C**** MAGNETIC: NON-MIDPLANE REGION
C****
      CALL BEY( BEF,Z,X,Y,0 )
      BX = BEF(1)
      BY = BEF(2)
      BZ = BEF(3)
      GO TO 4
C****
C**** MAGNETIC:     MIDPLANE REGION
C****
    3 CONTINUE
      IF( NFLAG .EQ. 0 ) GO TO 6
      SINE = -1.
      IF( IN .EQ. 3 ) SINE=1.
      DR = X*SINE
    6 CALL BEFN(B0,Z,X,Y,DR,0)
      BX = 0.
      BY = B0
      BZ = 0.
C****
C**** NOW FIND E FIELD
C****
    4 IF ( X .EQ. 0 ) GO TO 5
C****
C**** ELECTRIC: NON-MIDPLANE REGION
C****
      CALL BEY( BEF,Z,Y,X,1 )
      EX = BEF(2)
      EY = BEF(1)
      EZ = BEF(3)
      RETURN
C****
C**** ELECTRIC:     MIDPLANE REGION
C****
    5 CALL BEFN ( B1,Z,Y,X,0.0d0,1 )
      EX = B1
      EY = 0.
      EZ = 0.
      RETURN
      END

      SUBROUTINE BEY (BEF,Z,X,Y,IBEX )
C****
C**** CALCULATE B OR E FIELD OFF THE MEDIAN PLANE
C****
C****
C****
C**** IBEX = 0   MAGNETIC FIELD COMPONENTS
C****      = 1   ELECTRIC FIELD COMPONENTS
C****
      IMPLICIT  REAL*8 (A-H,O-Z)
      REAL*8 NDX
      COMMON /BLCK73/  IN,NFLAG
      COMMON /BLCK74/  BF,EF,S,DG
      COMMON /BLCK77/  RB,NDX
      DIMENSION BEF(3)
C****
C****
C**** NON MID-PLANE FRINGING FIELD REGION
C****
      IF( IBEX  .EQ. 1 ) GO TO 1
      IF( NFLAG .EQ. 0 ) GO TO 1
      SINE = -1.
      IF( IN .EQ. 3 ) SINE=1.
      DR0  = X*SINE
      DR1  = SINE* X
      DR2  = DR1
      DR9  = DR1
      DR10 = DR1
      DR3  = SINE* ( X + DG )
      DR5  = DR3
      DR11 = DR3
      DR4  = SINE*( X - DG )
      DR7  = DR4
      DR12 = DR4
      DR6  = SINE* ( X + 2.*DG )
      DR8  = SINE* ( X - 2.*DG )
C****
C****
C****
    1 CALL BEFN(F0,Z,X,Y,       DR0, IBEX )
      CALL BEFN(F1,Z+DG,X,Y,    DR1, IBEX )
      CALL BEFN(F2,Z+2.*DG,X,Y, DR2, IBEX )
      CALL BEFN(F3,Z+DG,X+DG,Y, DR3, IBEX )
      CALL BEFN(F4,Z+DG,X-DG,Y, DR4, IBEX )
      CALL BEFN(F5,Z   ,X+DG,Y, DR5, IBEX )
      CALL BEFN(F6,Z,X+2.*DG,Y, DR6, IBEX )
      CALL BEFN(F7,Z,X-DG,Y,    DR7, IBEX )
      CALL BEFN(F8,Z,X-2.*DG,Y, DR8, IBEX )
      CALL BEFN(F9,Z-DG,X,Y,    DR9, IBEX )
      CALL BEFN(F10,Z-2.*DG,X,Y,DR10,IBEX )
      CALL BEFN(F11,Z-DG,X+DG,Y,DR11,IBEX )
      CALL BEFN(F12,Z-DG,X-DG,Y,DR12,IBEX )
C****
      YG1 = Y/DG
      YG2 = YG1**2
      YG3 = YG1**3
      YG4 = YG1**4
C****
      BEF(1) = YG1 * ( (F5-F7)*2./3. - (F6-F8)/12. ) +
     1         YG3 * ( (F5-F7)/6. - (F6-F8)/12. -
     2         ( F3 + F11 - F4 - F12 - 2.*F5 + 2.*F7 )/12. )
      BEF(2) = F0 - YG2*( (F1 + F9 + F5 + F7 - 4.*F0) * 2./3. -
     1         ( F2 + F10 + F6 + F8 - 4.*F0 )/24. ) +
     2         YG4 * (-( F1 + F9 + F5 + F7 - 4.*F0 )/6. +
     3         ( F2 + F10 +      F6 + F8 - 4.*F0 )/24. +
     4         ( F3 + F11 + F4 + F12 - 2.*F1 - 2.*F9 -
     5         2.*F5 - 2.*F7 + 4.*F0 )/12. )
      BEF(3) = YG1 * ( (F1 - F9)*2./3. - (F2 - F10)/12. ) +
     1         YG3 * ( (F1 - F9)/6. - (F2 - F10)/12. -
     2         (F3 + F4 - F11 - F12 - 2.*F1 + 2.*F9)/12. )
      RETURN
      END


      SUBROUTINE BFLD
C****
C**** CALCULATION OF FIELD COMPONENTS FOR EACH PURE MULTIPOLE
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 K
      COMMON  /BLCK 7/ NCODE
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK50/  D, GRAD, S, BT
      COMMON  /BLCK51/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK52/  IN
      DIMENSION TC(6), DTC(6)
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      GO TO ( 11, 12, 13, 14 ) , NCODE
C****
C**** QUADRUPOLE
C****
   11 CONTINUE
      GO TO ( 2, 1, 2 ) , IN
c      write (6, 3) IN
    3 FORMAT( '  ERROR IN BQUAD  IN= ',I5 ///)
      CALL EXIT
    1 BX = GRAD*Y
      BY = GRAD*X
      BZ = 0.
      BT =   DSQRT( BX*BX + BY*BY )
      RETURN
    2 S = Z/D
      CS = C0 + C1*S + C2*S**2 + C3*S**3 + C4*S**4 + C5*S**5
      CSP = C1 + 2.*C2*S + 3.*C3*S**2 + 4.*C4*S**3 + 5.*C5*S**4
      CSPP = 2.*C2 + 6.*C3*S + 12.*C4*S**2 + 20.*C5*S**3
      IF( DABS(CS) .GT. 70. )  CS = DSIGN(70.D0, CS )
      E = DEXP(CS)
      RE = 1./(1. + E)
      CB1 = GRAD*RE
      CB2 = CB1*E*RE*( CSP**2 + CSPP - 2.*E*RE*CSP**2 )/(12.*D*D )
      BX = CB1*Y + CB2*( 3.*X*X + Y*Y ) * Y
      BY = CB1*X + CB2*( 3.*Y*Y + X*X ) * X
      BZ = -CB1*E*CSP*RE*X*Y / D
      BT =   DSQRT( BX*BX + BY*BY + BZ*BZ )
      RETURN
C****
C**** HEXAPOLE
C****
   12 BA2 = GRAD
      GO TO ( 22, 21, 22 ) , IN
c      write (6, 23) IN
   23 FORMAT( '  ERROR IN BHEX   IN= ',I5 ///)
      CALL EXIT
   21 BX = 2.*BA2*X*Y
      BY = BA2*( X*X - Y*Y )
      BZ = 0.
      BT =   DSQRT( BX*BX + BY*BY )
      RETURN
   22 S = Z/D
      IF( S .LT. 0. ) GO TO 21
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      RETURN
C****
C**** OCTAPOLE
C****
   13 BA3 = GRAD
      GO TO ( 32, 31, 32 ) , IN
c      write (6, 33) IN
   33 FORMAT( '  ERROR IN BOCT   IN= ',I5 ///)
      CALL EXIT
   31 BX = BA3*( 3.*X*X*Y - Y**3 )
      BY = BA3*( X**3 - 3.*X*Y*Y )
      BZ = 0.
      BT =   DSQRT( BX*BX + BY*BY )
      RETURN
   32 S = Z/D
      IF( S .LT. 0. ) GO TO 31
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      RETURN
C****
C**** DECAPOLE
   14 BA4 = GRAD
      GO TO ( 42, 41, 42 ) , IN
c      write (6, 43) IN
   43 FORMAT( '  ERROR IN BDEC   IN= ',I5 ///)
      CALL EXIT
   41 BX = 4.D0*BA4*( X**3 *Y - X*(Y**3) )
      BY = BA4*( X**4 - 6.D0* X*X*Y*Y + Y**4  )
      BZ = 0.
      BT =   DSQRT( BX*BX + BY*BY )
      RETURN
   42 S = Z/D
      IF( S .LT. 0. ) GO TO 41
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      RETURN
      END


      SUBROUTINE BMULT
C****
C****
C**** THE RELATIONSHIP BETWEEN B0, ......... B12 AND B(I,J) RELATIVE TO
C**** AXES (Z,X) IS GIVEN BY
C****
C****
C****
C**** B0  = B( 0, 0 )
C**** B1  = B( 1, 0 )
C**** B2  = B( 2, 0 )
C**** B3  = B( 1, 1 )
C**** B4  = B( 1,-1 )
C**** B5  = B( 0, 1 )
C**** B6  = B( 0, 2 )
C**** B7  = B( 0,-1 )
C**** B8  = B( 0,-2 )
C**** B9  = B(-1, 0 )
C**** B10 = B(-2, 0 )
C**** B11 = B(-1, 1 )
C**** B12 = B(-1,-1 )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  K, L
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLK100/  W, L, D, DG, S, BF, BT
      COMMON  /BLK101/  C0, C1, C2, C3, C4, C5, C6, C7, C8
      DIMENSION TC(6), DTC(6)
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      CALL MLTT ( B0, Z, X, Y )
      CALL MLTT ( B1 , Z + DG, X , Y )
      CALL MLTT ( B2 , Z + 2.*DG, X , Y )
      CALL MLTT ( B3 , Z + DG, X + DG , Y )
      CALL MLTT ( B4 , Z + DG, X - DG , Y )
      CALL MLTT ( B5 , Z , X + DG , Y )
      CALL MLTT ( B6 , Z , X + 2.*DG , Y )
      CALL MLTT ( B7 , Z , X - DG , Y )
      CALL MLTT ( B8 , Z , X - 2.*DG , Y )
      CALL MLTT ( B9 , Z - DG, X , Y )
      CALL MLTT ( B10, Z - 2.*DG, X , Y )
      CALL MLTT ( B11, Z - DG, X + DG , Y )
      CALL MLTT ( B12, Z - DG, X - DG , Y )
      YG1 = Y/DG
      YG2 = YG1**2
      YG3 = YG1**3
      YG4 = YG1**4
      BX = YG1 * ( (B5-B7)*2./3. - (B6-B8)/12. )  +
     1     YG3*( (B5-B7)/6. - (B6-B8)/12. -
     2     (B3 + B11 - B4 - B12 - 2.*B5 + 2.*B7 ) / 12. )
      BY = B0 - YG2*( ( B1 + B9 + B5 + B7 - 4.*B0 ) *2./3. -
     1     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. ) +
     2     YG4* (-( B1 + B9 + B5 + B7 - 4.*B0 ) / 6. +
     3     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. +
     4     ( B3 + B11 + B4 + B12 - 2.*B1 - 2.*B9 -
     5     2.*B5 - 2.*B7 + 4.*B0 ) / 12. )
      BZ = YG1*( (B1 - B9 ) *2./3. - ( B2 - B10 ) /12. ) +
     1     YG3*( ( B1 - B9 ) / 6. - ( B2 - B10 ) / 12. -
     2     ( B3 + B4 - B11 - B12 - 2.*B1 + 2.*B9 ) / 12.  )
      BT  =DSQRT(BX*BX + BY*BY + BZ*BZ)
      RETURN
      END


      SUBROUTINE BPLS ( IGP, D, S, RE, G1, G2, G3, G4, G5, G6 )
C****
C****
C****
      IMPLICIT REAL*8 (A-H,O-Z)
C****
C****
      COMMON  /BLCK91/  C0, C1, C2, C3, C4, C5
C****
C****
      S2 = S*S
      S3 = S2*S
      S4 = S2*S2
      S5 = S4*S
      CS = C0 + C1*S + C2*S2 + C3*S3 + C4*S4 + C5*S5
      CP1 =(C1 + 2.*C2*S + 3.*C3*S2 + 4.*C4*S3 + 5.*C5*S4) / D
      CP2 = (2.*C2 + 6.*C3*S + 12.*C4*S2 + 20.*C5*S3  ) / (D*D)
      CP3 = ( 6.*C3 + 24.*C4*S + 60.*C5*S2 ) / (D**3)
      CP4 = ( 24.*C4 + 120.*C5*S ) / (D**4)
C****
      CP5 = 120.*C5/(D**5)
C****
C****
C****
      IF( DABS(CS) .GT. 70. )  CS = DSIGN(70.D0, CS )
      E = DEXP(CS)
      RE = 1./(1. + E)
      ERE = E*RE
      ERE1= ERE*RE
      ERE2= ERE*ERE1
      ERE3= ERE*ERE2
      ERE4= ERE*ERE3
C****
      ERE5= ERE*ERE4
      ERE6= ERE*ERE5
C****
C****
      CP12 = CP1*CP1
      CP13 = CP1*CP12
      CP14 = CP12*CP12
      CP22 = CP2*CP2
C****
      CP15 = CP12*CP13
      CP16 = CP13*CP13
      CP23 = CP2*CP22
      CP32 = CP3*CP3
C****
C****
      IF( IGP .EQ. 6 ) RETURN
      G1 = -CP1*ERE1
C****
C****
      IF( IGP .EQ. 5 ) RETURN
      IF( IGP .EQ. 4 ) GO TO 1
      G2 =-( CP2+CP12   )*ERE1    + 2.*CP12 * ERE2
      G3 =-(CP3 + 3.*CP1*CP2 + CP13  ) * ERE1      +
     1   6.*(CP1*CP2 + CP13)*ERE2 - 6.*CP13*ERE3
C****
C****
      IF( IGP .EQ. 3 ) RETURN
1     G4 = -(CP4 + 4.*CP1*CP3 + 3.*CP22 + 6.*CP12*CP2 + CP14)*ERE1  +
     1   (8.*CP1*CP3 + 36.*CP12*CP2 + 6.*CP22 + 14.*CP14)*ERE2    -
     2   36.*(CP12*CP2 + CP14)*ERE3       + 24.*CP14*ERE4
C****
C****
      IF( IGP .NE. 2 ) RETURN
      G5 = (-CP5 - 5.*CP1*CP14 - 10.*CP2*CP3 - 10.*CP12*CP3 -
     1     15.*CP1*CP22 - 10.*CP13*CP2 - CP15)*ERE1 +
     2     (10.*CP1*CP4 +20.*CP2*CP3 +60.*CP12*CP3 + 90.*CP1*CP22 +
     3     140.*CP13*CP2 +30.*CP15)*ERE2 + (-60.*CP12*CP3 -
     4     90.*CP1*CP22 - 360.*CP13*CP2 - 150.*CP15)*ERE3 +
     5     (240.*CP13*CP2 +240.*CP15)*ERE4 + (-120.*CP15)*ERE5
      G6 = (-6.*CP1*CP5 - 15.*CP2*CP4 - 15.*CP12*CP4 - 10.*CP32 -
     1     60.*CP1*CP2*CP3 - 20.*CP13*CP3 - 15.*CP23 - 45.*CP12*CP22 -
     2     15.*CP14*CP2 - CP16)*ERE1 + (12.*CP1*CP5 + 30.*CP2*CP4 +
     3     90.*CP12*CP4 +20.*CP32 + 360.*CP1*CP2*CP3 +280.*CP13*CP3 +
     4     90.*CP23 + 630.*CP12*CP22 + 450.*CP14*CP2 + 62.*CP16)*ERE2 +
     5     (-90.*CP12*CP4 - 360.*CP1*CP2*CP3 -720.*CP13*CP3 -90.*CP23 -
     6     1620.*CP12*CP22 -2250.*CP14*CP2 - 540.*CP16)*ERE3 +
     7     (480.*CP13*CP3 + 1080.*CP12*CP22 + 3600.*CP14*CP2 +
     8     1560.*CP16)*ERE4 + (-1800.*CP14*CP2 - 1800.*CP16)*ERE5 +
     9     720.*CP16*ERE6
C****
      RETURN
      END


      SUBROUTINE BPOLES
C****
C**** CALCULATION OF MULTIPOLE(POLES) FIELD COMPONENTS
C****
C****
C****
C**** 2 - QUADRUPOLE  (GRAD1)
C**** 3 - HEXAPOLE    (GRAD2)
C**** 4 - OCTAPOLE    (GRAD3)
C**** 5 - DECAPOLE    (GRAD4)
C**** 6 - DODECAPOLE  (GRAD5)
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK90/  D, S, BT, GRAD1,GRAD2,GRAD3,GRAD4,GRAD5
      COMMON  /BLCK91/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK92/  IN
      COMMON  /BLCK93/  DH, DO, DD, DDD, DSH, DSO, DSD, DSDD
      DIMENSION TC(6), DTC(6)
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      X2 = X*X
      X3 = X2*X
      X4 = X3*X
      X5 = X4*X
      X6 = X5*X
      X7 = X6*X
      Y2 = Y*Y
      Y3 = Y2*Y
      Y4 = Y3*Y
      Y5 = Y4*Y
      Y6 = Y5*Y
      Y7 = Y6*Y
      GO TO ( 2, 1, 2 ) , IN
c      write(6,3) IN
    3 FORMAT( '  ERROR IN BPOLES IN= ',I5,///)
      CALL EXIT
    1 CONTINUE
      B2X = GRAD1*Y
      B2Y = GRAD1*X
      B3X = GRAD2*2.*X*Y
      B3Y = GRAD2*(X2-Y2)
      B4X = GRAD3*(3.*X2*Y-Y3)
      B4Y = GRAD3*(X3-3.*X*Y2)
      B5X = GRAD4*4.*(X3*Y-X*Y3)
      B5Y = GRAD4*(X4-6.*X2*Y2+Y4)
      B6X = GRAD5*(5.*X4*Y-10.*X2*Y3+Y5)
      B6Y = GRAD5*(X5-10.*X3*Y2+5.*X*Y4)
      BX = B2X + B3X + B4X + B5X + B6X
      BY = B2Y + B3Y + B4Y + B5Y + B6Y
      BZ = 0.
      BT =   DSQRT( BX*BX + BY*BY )
      RETURN
C****
C****
C****
    2 S = Z/D
      CALL BPLS( 2, D, S, RE, G1, G2, G3, G4, G5, G6 )
      B2X = GRAD1*( RE*Y - (G2/12.)*(3.*X2*Y + Y3) +
     1   (G4/384.)*(5.*X4*Y + 6.*X2*Y3 + Y5 ) -
     2   (G6/23040.)*(7.*X6*Y + 15.*X4*Y3 + 9.*X2*Y5 + Y7)  )
      B2Y = GRAD1*( RE*X - (G2/12.)*(X3 + 3.*X*Y2) +
     1   (G4/384.)*(X5 + 6.*X3*Y2 + 5.*X*Y4 ) -
     2   (G6/23040.)*(X7 + 9.*X5*Y2 + 15.*X3*Y4 + 7.*X*Y6) )
      B2Z = GRAD1*( G1*X*Y - (G3/12.)*(X3*Y + X*Y3 ) +
     1   (G5/384.)*(X5*Y +2.*X3*Y3 + X*Y5)  )
C****
C****
      SS = Z/DH  + DSH
      CALL BPLS( 3, DH, SS, RE, G1, G2, G3, G4, G5, G6 )
      B3X = GRAD2*( RE*2.*X*Y - (G2/48.)*(12.*X3*Y + 4.*X*Y3 ) )
      B3Y = GRAD2*( RE*(X2-Y2) - (G2/48.)*(3.*X4 + 6.*X2*Y2 - 5.*Y4 ) )
      B3Z = GRAD2*( G1*(X2*Y - Y3/3.) - (G3/48.)*(3.*X4*Y+2.*X2*Y3-Y5))
C****
C****
      SS = Z/DO  + DSO
      CALL BPLS( 4, DO, SS, RE, G1, G2, G3, G4, G5, G6 )
      B4X = GRAD3*( RE*(3.*X2*Y - Y3) - (G4/80.)*(20.*X4*Y - 4.*Y5 ) )
      B4Y = GRAD3*( RE*(X3 - 3.*X*Y2) - (G4/80.)*(4.*X5-20.*X*Y4 ) )
      B4Z = GRAD3*G1*(X3*Y - X*Y3 )
C****
C****
      SS = Z/DD  + DSD
      CALL BPLS( 5, DD, SS, RE, G1, G2, G3, G4, G5, G6 )
      B5X = GRAD4*RE*(4.*X3*Y - 4.*X*Y3)
      B5Y = GRAD4*RE*(X4 - 6.*X2*Y2 + Y4 )
      B5Z = GRAD4*G1*(X4*Y - 2.*X2*Y3 + Y5/5. )
C****
C****
      SS = Z/DDD + DSDD
      CALL BPLS( 6, DDD,SS, RE, G1, G2, G3, G4, G5, G6 )
      B6X = GRAD5*RE*(5.*X4*Y - 10.*X2*Y3 + Y5 )
      B6Y = GRAD5*RE*(X5 - 10.*X3*Y2 + 5.*X*Y4 )
      B6Z = 0.
C****
C****
      BX = B2X + B3X + B4X + B5X + B6X
      BY = B2Y + B3Y + B4Y + B5Y + B6Y
      BZ = B2Z + B3Z + B4Z + B5Z + B6Z
      BT =   DSQRT( BX*BX + BY*BY + BZ*BZ )
      RETURN
      END


      SUBROUTINE BPRETZ
C****
C****
C**** MTYP=6
C****
C****
C**** PRETZEL MAGNET FIELD COMPONENTS
C**** DG = SMALL NEGATIVE NUMBER
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  NDX, K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
C****
C****
      G1 = BF/D
      Y = TC(2)
      Z = TC(3)
      IF( Z .LE. DG ) GO TO 1
      BX = 0.
      BY = 0.
      BZ = 0.
      RETURN
    1 BY0 = G1*DABS(Z)**NDX
      BY1 = BY0*NDX/Z
      BY2 = BY1*(NDX-1.)/Z
      BY3 = BY2*(NDX-2.)/Z
      BY4 = BY3*(NDX-3.)/Z
      BX = 0.
      BY = BY0 - Y*Y*BY2/2. + Y**4*BY4/24.
      BZ = Y*BY1 - Y**3*BY3/6.
      BT = DSQRT(BX*BX + BY*BY + BZ*BZ)
      RETURN
      END


      SUBROUTINE BSOL
C****
C****
C**** ROUTINE VALID FOR FIELDS OUTSIDE CENTRAL ZONE OF ELEMENTAL
C**** SOLENOID
C**** BF    = FIELD AT CENTER OF INFINITE SOLENOID; CURR. DEN. (NI/M)
C**** M.W.GARRETTT  JOURNAL OF APP. PHYS. 34,(1963),P2567
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 K
      DIMENSION  TC(6), DTC(6)
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK30/  BF ,      AL, RAD
      COMMON  /BLCK31/  S, BT
      COMMON  /BLCK32/  IN
C****
C****
      DATA PI4/12.566370616D0 /
C****
C****
C****
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      R =DSQRT( X **2 + Y**2 )
      IF( R  .LT.  (RAD/1.D4)  )  GO TO 5
      RADR = RAD+R
      AAPR = 4.D0*RAD/RADR
      AAMR = (RAD-R)/(2.D0*RAD)
      RCSQ = 4.D0*RAD*R/(RADR*RADR)
C****
C**** SOLENOID LEFT  HAND SOURCE
C****
      ZZ = -(AL+Z)
      R1SQ = RADR*RADR  + ZZ*ZZ
      R1 = DSQRT(R1SQ)
      RKSQ = 4.D0*RAD*R/R1SQ
      CALL FB01AD(RKSQ,       VKS, VES )
      CALL FB03AD(RCSQ, RKSQ, P )
      BZS1 = AAPR*ZZ*(VKS+AAMR*(P-VKS) ) /R1
      BRS1 = R1*(2.D0*(VKS-VES) - RKSQ*VKS)
C****
C**** SOLENOID RIGHT HAND SOURCE
C****
      ZZ = AL-Z
      R1SQ = RADR*RADR  + ZZ*ZZ
      R1 = DSQRT(R1SQ)
      RKSQ = 4.D0*RAD*R/R1SQ
      CALL FB01AD(RKSQ,       VKS, VES )
      CALL FB03AD(RCSQ, RKSQ, P )
      BZS2 = AAPR*ZZ*(VKS+AAMR*(P-VKS) ) /R1
      BRS2 = R1*(2.D0*(VKS-VES) - RKSQ*VKS)
      BZ = BF*( BZS2-BZS1 )/PI4
      BR = BF*( BRS2-BRS1 )/(R*PI4)
      BX = BR * X /R
      BY = BR *  Y/R
      BT =DSQRT( BX**2 + BY**2 + BZ**2 )
      RETURN
    5 CONTINUE
C****
C****
C****
      COSA = (AL-Z) / DSQRT( RAD*RAD + (AL-Z)**2  )
      COSB =-(AL+Z) / DSQRT( RAD*RAD + (AL+Z)**2  )
      BX = 0.
      BY = 0.
      BZ = BF*(COSA-COSB)/2.D0
      BT = DABS(BZ)
      RETURN
      END


      SUBROUTINE COLL ( NO, J, IFLAG  )
C****
C****
C**** TEST AND SET FLAG IF RAY EXCEEDS RECTANGULAR OR ELLIPTICAL
C**** COLLIMATOR CUT-OFF DIMENSIONS
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 2/  XO, YO, ZO, VXO, VYO, VZO,RTL(1000),RLL(1000)
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
      DIMENSION DATA(75,200),ITITLE(200)
C****
C****
  100 FORMAT( // 5X, 'RAY=', I5, 5X, 'ELEMENT=', I3,
     1   '   STOPPED - EXCEEDS RECTANGULAR COLLIMATOR DIMENSIONS ' // )
  101 FORMAT( // 5X, 'RAY=', I5, 5X, 'ELEMENT=', I3,
     1   '   STOPPED - EXCEEDS ELLIPTICAL  COLLIMATOR DIMENSIONS ' // )
C****
C****
      IFLAG = 0
      ICOLL = DATA(1,NO)
      XCEN  = DATA(2,NO)
      YCEN  = DATA(3,NO)
      XMAX  = DATA(4,NO)
      YMAX  = DATA(5,NO)
      IF ( ICOLL .NE. 0 ) GO TO 1
      IF ( (DABS(XA-XCEN) .GT. XMAX) .OR. (DABS(YA-YCEN) .GT. YMAX) )
     1      GO TO 2
      RETURN
    2 CONTINUE
c    2 write (6, 100) J, NO
      GO TO 3
    1 XC = (XA-XCEN)/XMAX
      YC = (YA-YCEN)/YMAX
      IF ( (XC*XC+YC*YC) .GT. 1. ) GO TO 4
      RETURN
    4 CONTINUE
c    4 write (6, 101) J, NO
    3 XO(J)  = 1.D10
      YO(J)  = 1.D10
      VXO(J) = 0.
      VYO(J) = 0.
      IFLAG  = 1
      RETURN
      END

      SUBROUTINE  DERIV( BFUN )
C****
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  K
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK11/  EX, EY, EZ, QMC, IVEC
      DIMENSION TC(6), DTC(6)
      DATA  C /3.D10 /
C****
C****
      CALL BFUN
      DTC(1) = TC(4)
      DTC(2) = TC(5)
      DTC(3) = TC(6)
      IF( IVEC .NE. 0 )  GO TO 4
      DTC(4) = K * ( TC(5) * BZ - TC(6) * BY )
      DTC(5) = K * ( TC(6) * BX - TC(4) * BZ )
      DTC(6) = K * ( TC(4) * BY - TC(5) * BX )
      RETURN
    4 VEL = DSQRT( TC(4)**2 + TC(5)**2 + TC(6)**2 )
C****
C**** SK 12/02/83
C**** GAMMA CORRECTION FOR HIGH ENERGY ELECTRONS
C**** NOT EXACT
C****
      GAMMA = 1.D0 + ENERGY/(PMASS*931.48D0)
      IF( GAMMA .LT. 100. ) GAMMA = 1./DSQRT( 1.-VEL*VEL/(C*C) )
C****
C****
      K = 1./(QMC*GAMMA)
      AK = K/(9.D13)
      ETERM = (EX*TC(4)+EY*TC(5)+EZ*TC(6) )*AK
      DTC(4) = K*( TC(5)*BZ - TC(6)*BY + EX*1.D7 ) - TC(4)*ETERM
      DTC(5) = K*( TC(6)*BX - TC(4)*BZ + EY*1.D7 ) - TC(5)*ETERM
      DTC(6) = K*( TC(4)*BY - TC(5)*BX + EZ*1.D7 ) - TC(6)*ETERM
      RETURN
      END


      SUBROUTINE DIPOLE ( NO, NP, T, TP ,NUM )
C****
C****
C**** SINGLE MAGNET RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF1, LF2, LU1, K, NDX
      EXTERNAL BDIP
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION DATA(  75,200 ) ,ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
C**** DATA  C/ 3.D10/
C****
      LF1  = DATA(  1,NO )
      LU1  = DATA(  2,NO )
      LF2  = DATA(  3,NO )
      DG   = DATA(  4,NO )
      MTYP = DATA(  5,NO )
      A    = DATA( 11,NO )
      B    = DATA( 12,NO )
      D    = DATA( 13,NO )
      RB   = DATA( 14,NO )
      BF   = DATA( 15,NO )
      PHI  = DATA( 16,NO )
      ALPHA= DATA( 17,NO )
      BETA = DATA( 18,NO )
      NDX  = DATA( 19,NO )
      BET1 = DATA( 20,NO )
      GAMA = DATA( 21,NO )
      DELT = DATA( 22,NO )
      Z11  = DATA( 25,NO )
      Z12  = DATA( 26,NO )
      Z21  = DATA( 27,NO )
      Z22  = DATA( 28,NO )
      BR1  = DATA( 41,NO )
      BR2  = DATA( 42,NO )
      XCR1 = DATA( 43,NO )
      XCR2 = DATA( 44,NO )
      IF( MTYP .EQ. 0  )  MTYP = 1
      DTF1= LF1/ VEL
      DTF2= LF2/ VEL
      DTU = LU1/ VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S = 0.
      BR = BR1
      IF( NP  .GT. 100 ) GO TO 5
c      write (6, 100) ITITLE(NO)
  100 FORMAT(  ' DIPOLE  ****  ', A4,'  ****************************'/)
c      write (6, 101)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HBX, 8X, 4HY CM , 7X, 2HBY,
     1   8X, 4HZ CM, 7X, 2HBZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HB             )
      CALL PRNT2 ( T,S,XA   ,YA   ,ZA   ,BX,BY,BZ,BT,VXA  ,VYA  ,VZA   )
c      write (6, 103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO B AXIS SYSTEM '       )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO VFB COORD.
C****
    5 COSA =DCOS( ALPHA/57.29578)
      SINA =DSIN( ALPHA/57.29578)
      TC(1) = ( A-ZA ) * SINA - ( XA + XCR1 ) * COSA
      TC(2) = YA
      TC(3) = ( A-ZA ) * COSA + ( XA + XCR1 ) * SINA
      TC(4) = -VZA * SINA - VXA * COSA
      TC(5) = VYA
      TC(6) = -VZA * COSA + VXA * SINA
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FIRST FRINGE FIELD
C****
C****
      IF(  BR1  .EQ.  0. ) GO TO 20
      IN = 4
      XDTF1 = DTF1
      IF(  Z11  .GT.  TC(3) )  XDTF1 = -DTF1
c      IF( NP  .LE. 100) write (6, 108)
  108 FORMAT(/ ' CONSTANT FIELD CORRECTION IN FRINGE FIELD REGION    ' )
      NSTEP = 0
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  0    )
   21 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 22  I=1,NP
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF(  XDTF1  .LT.  0. )  GO TO 23
      IF(  Z11  .GE.  TC(3) )  GO TO 24
      GO TO 22
   23 IF(  Z11  .LE.  TC(3) )  GO TO 24
   22 CONTINUE
      GO TO 21
   24 DO 2 I=1,2
      XDTF1 = (TC(3) - Z11) / DABS(TC(6))
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  0    )
    2 CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C****
C****
C****
   20 TDT = ( TC(3) - Z11 ) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** IN DESIGNATES MAGNET REGIONS FOR BFUN
C****
      IN = 1
      XC= RB*DCOS( ALPHA/ 57.29578 )
      ZC=-RB*DSIN( ALPHA/ 57.29578 )
C****
      C0   = DATA( 29,NO )
      C1   = DATA( 30,NO )
      C2   = DATA( 31,NO )
      C3   = DATA( 32,NO )
      C4   = DATA( 33,NO )
      C5   = DATA( 34,NO )
      DELS = DATA( 45,NO )
      RCA  = DATA( 47,NO )
      CSC = DCOS( ALPHA/57.29578 )
      SCOR = DATA(49,NO)
      S2   = DATA( 51,NO ) / RB    + RCA/2.D0
      S3   = DATA( 52,NO ) / RB**2
      S4   = DATA( 53,NO ) / RB**3 + RCA**3/8.D0
      S5   = DATA( 54,NO ) / RB**4
      S6   = DATA( 55,NO ) / RB**5 + RCA**5/16.D0
      S7   = DATA( 56,NO ) / RB**6
      S8   = DATA( 57,NO ) / RB**7 + RCA**7/25.6D0
c      IF( NP  .LE. 100) write (6, 104)
  104 FORMAT( 22H0FRINGING FIELD REGION    )
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BDIP,  0    )
      NSTEP = 0
    6 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( Z12 .GE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 =-( Z12 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  0    )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
  105 FORMAT( 10H   NSTEPS= ,I5 )
C***
C***  UNIFORM FIELD REGION
C**** TRANSFORM TO SECOND VFB COORD SYSTEM
C***
      COPAB =DCOS( (PHI-ALPHA-BETA)/57.29578)
      SIPAB =DSIN( (PHI-ALPHA-BETA)/57.29578)
      COSPB =DCOS( (PHI/2.-BETA)/57.29578 )
      SINPB =DSIN( (PHI/2.-BETA)/57.29578 )
      SIP2 =DSIN( (PHI/2.)/57.29578 )
      XT = TC(1)
      ZT = TC(3)
      VXT = TC(4)
      VZT = TC(6)
      TC(3) = - ZT  *COPAB +  XT  *SIPAB -2.*RB*SIP2*COSPB
      TC(1) = - ZT  *SIPAB -  XT  *COPAB -2.*RB*SIP2*SINPB
      TC(6) = - VZT *COPAB +  VXT *SIPAB
      TC(4) = - VZT *SIPAB -  VXT *COPAB
C****
C****
C**** UNIFORM FIELD INTEGRATION REGION
C****
C****
      IN = 2
      XC=-RB*DCOS( BETA / 57.29578 )
      ZC=-RB*DSIN( BETA / 57.29578 )
c      IF( NP  .LE. 100) write (6, 106)
  106 FORMAT(   '0UNIFORM FIELD REGION IN C AXIS SYSTEM '  )
      IF( TC(3)  .LT.  Z21 ) GO TO 15
C****
C**** THIS SECTION CORRECTS FOR MAGNETS WHOSE FRINGING FIELDS INTERSECT
C****
c      IF( NP  .LE. 100) write (6, 102)
  102 FORMAT( / '   INTEGRATE BACKWARDS    '  )
      CALL FNMIRK( 6, T,-DTU ,TC, DTC, DS, ES, BDIP,  0    )
      NSTEP = 0
   16 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 17  I =1, NP
      CALL FNMIRK( 6, T,-DTU, TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .LE.  Z21 )  GO TO 18
   17 CONTINUE
      GO TO 16
   18 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BDIP,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
c      IF( NP  .LE. 100) write (6, 107)
  107 FORMAT( / )
      GO TO 19
C****
C****
   15 CONTINUE
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, BDIP,  0    )
      NSTEP = 0
    9 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 10  I =1, NP
      CALL FNMIRK( 6, T, DTU, TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .GE.  Z21 )  GO TO 11
   10 CONTINUE
      GO TO 9
   11 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BDIP,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
   19 CONTINUE
C***
C***
C**** SETUP FOR SECOND FRINGE FIELD AND INTEGRATION
C****
C****
      BR   = BR2
      C0   = DATA( 35,NO )
      C1   = DATA( 36,NO )
      C2   = DATA( 37,NO )
      C3   = DATA( 38,NO )
      C4   = DATA( 39,NO )
      C5   = DATA( 40,NO )
      DELS = DATA( 46,NO )
      RCA  = DATA( 48,NO )
      SCOR = DATA(50,NO)
      CSC = DCOS( BETA /57.29578 )
      S2   = DATA( 58,NO ) / RB    + RCA/2.D0
      S3   = DATA( 59,NO ) / RB**2
      S4   = DATA( 60,NO ) / RB**3 + RCA**3/8.D0
      S5   = DATA( 61,NO ) / RB**4
      S6   = DATA( 62,NO ) / RB**5 + RCA**5/16.D0
      S7   = DATA( 63,NO ) / RB**6
      S8   = DATA( 64,NO ) / RB**7 + RCA**7/25.6D0
      IN = 3
c      IF( NP  .LE. 100) write (6, 104)
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BDIP,  0    )
      NSTEP = 0
   12 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 13  I =1, NP
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3) .GE. Z22 )  GO TO 14
   13 CONTINUE
      GO TO 12
   14 CONTINUE
      XDTF2 = ( Z22 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  0    )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      COSB =DCOS( BETA/57.29578 )
      SINB =DSIN( BETA/57.29578 )
      XT = TC(1)
      ZT = TC(3)
      VXT = TC(4)
      VZT = TC(6)
      TC(3) = ZT*COSB - XT*SINB - B
      TC(1) = ZT*SINB + XT*COSB - XCR2
      TC(6) = VZT*COSB - VXT*SINB
      TC(4) = VZT*SINB + VXT*COSB
c      IF( NP  .LE. 100) write (6, 109)
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
      IF(  BR2  .EQ.  0. ) GO TO 30
      IN = 4
      XDTF2 = DTF2
      IF( TC(3)  .GT. 0. ) XDTF2 = -DTF2
c      IF( NP  .LE. 100) write (6, 108)
      NSTEP = 0
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  0    )
   31 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 32  I=1,NP
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( XDTF2  .LT. 0. ) GO TO 33
      IF( TC(3)  .GE. 0. ) GO TO 34
      GO TO 32
   33 IF( TC(3)  .LE. 0. ) GO TO 34
   32 CONTINUE
      GO TO 31
   34 DO 3 I=1,2
      XDTF2 = -TC(3) / DABS(TC(6))
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  0    )
    3 CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BDIP,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C****
C****
C****
   30 TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF(NP.LE.100) write (6,115)TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF( NP .LE. 100) write (6, 111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=,F10.4, 3H MR   /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y=,F10.2, 3H CM   /       )
      RETURN
   99 CALL PRNT4(NO, IN)
      RETURN
      END


      SUBROUTINE  DRIFT( NO,  NP,T, TP ,NUM )
C****
C****
C**** Z-AXIS DRIFT ROUTINE
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      DIMENSION DATA(  75,200 ), ITITLE(200)
C**** DATA  C/ 3.D10/
  100 FORMAT( /  '   Z-AXIS DRIFT  ****  ', A4, '****************',//
     1'      T CM', 18X, 'X CM', 7X, 'Y CM', 7X, 'Z CM' , '      VELZ/C'
     2   , '    THETA MR      PHI MR'  /  )
  103 FORMAT( F10.4, 11X, 3F11.3, F12.5, 2F12.3  )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 1
      CALL PLT2 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 100) ITITLE(NO)
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
      VZP =  VZA  / VEL
      TP = T*VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      TDT =(DATA(1,NO) - ZA) / DABS(VZA)
      T = T + TDT
      TP = T*VEL
      XA = XA + TDT*VXA
      YA = YA + TDT*VYA
      ZA = 0.
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT2 ( NUM, NO, NBR, TPAR )
      RETURN
      END
C
C         SUBROUTINE DTIME
C         INTEGER *2 H,MI,S,Y,MO,D,X
C         CHARACTER COL,PNT,SL
C         DATA COL/':'/,PNT/'.'/,SL/'/'/
C         CALL GETTIM(H,MI,S,X)
C         CALL GETDAT(Y,MO,D)
C         WRITE (6,100) H,COL,MI,COL,S,PNT,X,MO,SL,D,SL,Y
C 100     FORMAT(5X,I2,A1,I2,A1,I2,A1,I2,5X,I2,A1,I2,A1,I4)
C         RETURN
C         END


      SUBROUTINE EDIP
C****
C****   CALCULATES E-FIELD COMPONENTS FOR A CYLINDRICAL
C****   ELECTROSTATIC DEFLECTOR
C****
      IMPLICIT REAL*8 (A-H, O-Z)
      REAL*8 K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK11/  EX, EY, EZ, QMC, IVEC
      COMMON  /BLCK20/ EC2, EC4, WE, WC
      COMMON  /BLCK22/  D, DG, S, EF, ET
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
C****
C****
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      DX = X - XC
      DZ = Z
      RP2 = DX * DX + Z * Z
      RP = DSQRT(RP2)
      GO TO (1, 2, 3) , IN
  100 FORMAT( ' ERROR -GO TO-  IN EDIP IN = ', I5)
c      write (6, 100) IN
C****
C**** UNIFORM FIELD REGION
C****
2     EX = - EF * RB * DX / RP2
      EY = 0.
      EZ = - EF * RB * Z / RP2
      ET = DSQRT(EX * EX + EZ * EZ)
      RETURN
C****
C**** FRINGE FIELD REGION
C****
1     CONTINUE
3     CONTINUE
      ZP1 = DZ + DG
      ZP2 = DZ + 2. * DG
      ZM1 = DZ - DG
      ZM2 = DZ - 2. * DG
      DRP1 = DSQRT( DX * DX + ZP1 * ZP1 )
      DRP2 = DSQRT( DX * DX + ZP2 * ZP2 )
      DRM1 = DSQRT( DX * DX + ZM1 * ZM1 )
      DRM2 = DSQRT( DX * DX + ZM2 * ZM2 )
      CALL EDPP (F0,   Z  ,  X, Y      , RP   )
      S0 = S
      CALL EDPP (F1,  ZP1 ,  X, Y      , DRP1 )
      CALL EDPP (F2,  ZP2 ,  X, Y      , DRP2 )
      CALL EDPP (F3,  ZP1 ,  X, Y+DG   , DRP1 )
      CALL EDPP (F4,  ZP1 ,  X, Y-DG   , DRP1 )
      CALL EDPP (F5,   Z  ,  X, Y+DG   , RP   )
      CALL EDPP (F6,   Z  ,  X, Y+2.*DG, RP   )
      CALL EDPP (F7,   Z  ,  X, Y-DG   , RP   )
      CALL EDPP (F8,   Z  ,  X, Y-2.*DG, RP   )
      CALL EDPP (F9,  ZM1 ,  X, Y      , DRM1 )
      CALL EDPP (F10, ZM2 ,  X, Y      , DRM2 )
      CALL EDPP (F11, ZM1 ,  X, Y+DG   , DRM1 )
      CALL EDPP (F12, ZM1 ,  X, Y-DG   , DRM1 )
      S = S0
      XG1 = X/DG
      XG2 = XG1*XG1
      XG3 = XG2*XG1
      XG4 = XG3*XG1
C****
      EY = XG1 * ( (F5-F7)*2./3. - (F6-F8)/12. ) +
     1         XG3 * ( (F5-F7)/6. - (F6-F8)/12. -
     2         ( F3 + F11 - F4 - F12 - 2.*F5 + 2.*F7 )/12. )
      EX = F0 - XG2*( (F1 + F9 + F5 + F7 - 4.*F0) * 2./3. -
     1         ( F2 + F10 + F6 + F8 - 4.*F0 )/24. ) +
     2         XG4 * (-( F1 + F9 + F5 + F7 - 4.*F0 )/6. +
     3         ( F2 + F10 +      F6 + F8 - 4.*F0 )/24. +
     4         ( F3 + F11 + F4 + F12 - 2.*F1 - 2.*F9 -
     5         2.*F5 - 2.*F7 + 4.*F0 )/12. )
      EZ = XG1 * ( (F1 - F9)*2./3. - (F2 - F10)/12. ) +
     1         XG3 * ( (F1 - F9)/6. - (F2 - F10)/12. -
     2         (F3 + F4 - F11 - F12 - 2.*F1 + 2.*F9)/12. )
      ET = DSQRT( EX * EX + EY * EY + EZ * EZ)
      RETURN
      END


      SUBROUTINE EDIPL( NO, NP, T, TP ,NUM )
C****
C****
C**** SINGLE MAGNET RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF1, LF2, LU1, K
      EXTERNAL EDIP
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK11/  EX, EY, EZ, QMC, IVEC
      COMMON  /BLCK20/ EC2, EC4, WE, WC
      COMMON  /BLCK22/  D, DG, S, EF, ET
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION DATA(  75,200 ) ,ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
C**** DATA  C/ 3.D10/
C****
      LF1  = DATA(  1,NO )
      LU1  = DATA(  2,NO )
      LF2  = DATA(  3,NO )
      DG   = DATA(  4,NO )
      A    = DATA( 11,NO )
      B    = DATA( 12,NO )
      D    = DATA( 13,NO )
      RB   = DATA( 14,NO )
      EF   = DATA( 15,NO )
      PHI  = DATA( 16,NO )
      EC2  = DATA( 17,NO )
      EC4  = DATA( 18,NO )
      WE   = DATA( 19,NO )
      WC   = DATA( 20,NO )
      Z11  = DATA( 25,NO )
      Z12  = DATA( 26,NO )
      Z21  = DATA( 27,NO )
      Z22  = DATA( 28,NO )
      DTF1= LF1/ VEL
      DTF2= LF2/ VEL
      DTU = LU1/ VEL
      IF (WE .EQ. 0.) WE = 1000. * RB
      BX = 0.
      BY = 0.
      BZ = 0.
      EX = 0.
      EY = 0.
      EZ = 0.
      ET = 0.
      S = 0.
      IF( NP  .GT. 100 ) GO TO 5
c      write (6,100) ITITLE(NO)
  100 FORMAT(  ' E.S.-DIPOLE ****', A4,'  ***************************'/)
c      write(6,101)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HEX, 8X, 4HY CM , 7X, 2HEY,
     1   8X, 4HZ CM, 7X, 2HEZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HE             )
      CALL PRNT5 ( T,S,XA   ,YA   ,ZA   ,EX,EY,EZ,ET,VXA  ,VYA  ,VZA   )
c      write(6,103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO B AXIS SYSTEM '       )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO EFB COORD.
C****
    5 CONTINUE
      TC(1) =  -  XA
      TC(2) = YA
      TC(3) = ( A-ZA )
      TC(4) = - VXA
      TC(5) = VYA
      TC(6) = -VZA
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C****
C****
   20 TDT = ( TC(3) - Z11 ) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** IN DESIGNATES MAGNET REGIONS FOR BFUN
C****
      IN = 1
      XC = RB
      ZC = 0.0
C****
      C0   = DATA( 29,NO )
      C1   = DATA( 30,NO )
      C2   = DATA( 31,NO )
      C3   = DATA( 32,NO )
      C4   = DATA( 33,NO )
      C5   = DATA( 34,NO )
c      IF( NP  .LE. 100) write(6,104)
  104 FORMAT( 22H0FRINGING FIELD REGION    )
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, EDIP,  0    )
      NSTEP = 0
    6 CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, EDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
        IF (NSTEP  .GT.  200)  GO TO 99
      IF( Z12 .GE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 =-( Z12 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, EDIP,  0    )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES, EDIP,  1    )
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      NUM = NUM + 1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write(6,105) NSTEP
  105 FORMAT( 10H   NSTEPS=,I5 )
C***
C***  UNIFORM FIELD REGION
C**** TRANSFORM TO SECOND EFB COORD SYSTEM
C***
      COPAB =DCOS( (PHI)/57.29578)
      SIPAB =DSIN( (PHI)/57.29578)
      COSPB =DCOS( (PHI/2.)/57.29578 )
      SINPB =DSIN( (PHI/2.)/57.29578 )
      SIP2 =DSIN( (PHI/2.)/57.29578 )
      XT = TC(1)
      ZT = TC(3)
      VXT = TC(4)
      VZT = TC(6)
      TC(3) = - ZT  *COPAB +  XT  *SIPAB -2.*RB*SIP2*COSPB
      TC(1) = - ZT  *SIPAB -  XT  *COPAB -2.*RB*SIP2*SINPB
      TC(6) = - VZT *COPAB +  VXT *SIPAB
      TC(4) = - VZT *SIPAB -  VXT *COPAB
C****
C****
C**** UNIFORM FIELD INTEGRATION REGION
C****
C****
      IN = 2
      XC = -RB
      ZC = 0.0
c      IF( NP  .LE. 100) write(6,106)
  106 FORMAT(   '0UNIFORM FIELD REGION IN C AXIS SYSTEM '  )
      IF( TC(3)  .LT.  Z21 ) GO TO 15
C****
C**** THIS SECTION CORRECTS FOR MAGNETS WHOSE FRINGING FIELDS INTERSECT
C****
c      IF( NP  .LE. 100) write(6,102)
  102 FORMAT( / '   INTEGRATE BACKWARDS    '  )
      CALL FNMIRK( 6, T,-DTU ,TC, DTC, DS, ES, EDIP,  0    )
      NSTEP = 0
   16 CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      DO 17  I =1, NP
      CALL FNMIRK( 6, T,-DTU, TC, DTC, DS, ES, EDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .LE.  Z21 )  GO TO 18
   17 CONTINUE
      GO TO 16
   18 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, EDIP,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, EDIP,  1    )
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write(6,105) NSTEP
c      IF( NP  .LE. 100) write(6,107)
  107 FORMAT( / )
      GO TO 19
C****
C****
   15 CONTINUE
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, EDIP,  0    )
      NSTEP = 0
    9 CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      DO 10  I =1, NP
      CALL FNMIRK( 6, T, DTU, TC, DTC, DS, ES, EDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .GE.  Z21 )  GO TO 11
   10 CONTINUE
      GO TO 9
   11 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, EDIP,  0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES, EDIP,  1    )
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write(6,105) NSTEP
   19 CONTINUE
C***
C***
C**** SETUP FOR SECOND FRINGE FIELD AND INTEGRATION
C****
C****
      C0   = DATA( 35,NO )
      C1   = DATA( 36,NO )
      C2   = DATA( 37,NO )
      C3   = DATA( 38,NO )
      C4   = DATA( 39,NO )
      C5   = DATA( 40,NO )
      IN = 3
c      IF( NP  .LE. 100) write(6,104)
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, EDIP,  0    )
      NSTEP = 0
   12 CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      DO 13  I =1, NP
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, EDIP,  1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3) .GE. Z22 )  GO TO 14
   13 CONTINUE
      GO TO 12
   14 CONTINUE
      XDTF2 = ( Z22 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, EDIP,  0    )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, EDIP,  1    )
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write(6,105) NSTEP
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      XT = TC(1)
      ZT = TC(3)
      VXT = TC(4)
      VZT = TC(6)
      TC(3) = ZT - B
      TC(1) = XT
      TC(6) = VZT
      TC(4) = VXT
c      IF( NP  .LE. 100) write(6,109)
      CALL PRNT5 ( T,S,TC(1),TC(2),TC(3),EX,EY,EZ,ET,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
C****
C****
C****
   30 TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      EX = 0.
      EY = 0.
      EZ = 0.
      ET = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF( NP.LE.100) write(6,115) TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF( NP  .LE. 100) write(6,111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=,F10.4, 3H MR   /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y=,F10.2, 3H CM   /     )
      RETURN
   99 CALL PRNT4(NO, IN)
      RETURN
      END


      SUBROUTINE EDPP( EFLD, Z, X, Y, DRP )
C****
C**** CALCULATE S; DETERMINE E-FIELD IN FRINGE REGIONS
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8 K
      COMMON  /BLCK20/ EC2, EC4, WE, WC
      COMMON  /BLCK22/  D, DG, S, EF, ET
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      FEF = -EF
      IF ( IN .EQ. 1 ) FEF = +EF
      W2 = WE * WE
      ZD1 = Z / D
      ZD2 = EC2 * ZD1 * Y * Y / W2
      W4 = W2 * W2
      ZD3 = EC4 * (Y**4) / W4
      S = ZD1 + ZD2 + ZD3
      CS = C0 + S * (C1 + S * (C2 + (S * (C3 + S * (C4 +S * C5)))))
      IF (DABS(CS) .GT. 70.) CS = DSIGN(70.D0, CS)
      E = DEXP(CS)
      P0 = 1.0 + E
      EFLD = (FEF / P0) * (RB / DRP)
      RETURN
      END
        SUBROUTINE EXIT
c       CALL DTIME
c	    ctemp=ctime(time())
c		write(6,*)ctemp
        STOP
        END


      SUBROUTINE FB01AD(C,  VK,VE)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA XLG/1.0D300/
C**NON-IBM......REAL * 8 XLG/'7FFFFFFFFFFFFFFF'X/
      D=1D0-C
      IF(D .GT. 0D0)E=-DLOG(D)
C**** HARWELL VERSION OF FB01AD
      IF(C .GE. 1D0)GO TO 2
      VE=E*((((((((((
     A     3.18591956555015718D-5*D  +.989833284622538479D-3)*D
     B    +.643214658643830177D-2)*D +.16804023346363385D-1)*D
     C    +.261450147003138789D-1)*D +.334789436657616262D-1)*D
     D    +.427178905473830956D-1)*D +.585936612555314917D-1)*D
     E    +.937499997212031407D-1)*D +.249999999999901772D0)*D)
     F    +(((((((((
     G     .149466217571813268D-3*D  +.246850333046072273D-2)*D
     H    +.863844217360407443D-2)*D+.107706350398664555D-1)*D
     I    +.782040406095955417D-2)*D +.759509342255943228D-2)*D
     J    +.115695957452954022D-1)*D +.218318116761304816D-1)*D
     K    +.568051945675591566D-1)*D +.443147180560889526D0)*D
     L    +1D0
C****
C**** ROUTINE MODIFIED TO CALCULATE VK AND VE ALWAYS
C****
C****
      VK=E*((((((((((
     A     .297002809665556121D-4*D   +.921554634963249846D-3)*D
     B    +.597390429915542916D-2)*D  +.155309416319772039D-1)*D
     C    +.239319133231107901D-1)*D  +.301248490128989303D-1)*D
     D    +.373777397586236041D-1)*D  +.48828041906862398D-1)*D
     E    +.703124997390383521D-1)*D  +.124999999999908081D0)*D
     F    +.5D0)+(((((((((
     G     .139308785700664673D-3*D   +.229663489839695869D-2)*D
     H    +.800300398064998537D-2)*D  +.984892932217689377D-2)*D
     I    +.684790928262450512D-2)*D  +.617962744605331761D-2)*D
     J    +.878980187455506468D-2)*D  +.149380135326871652D-1)*D
     K    +.308851462713051899D-1)*D  +.965735902808562554D-1)*D
     L    +1.38629436111989062D0
      RETURN
    2 VE=1D0
      VK=XLG
      RETURN
      END


      SUBROUTINE FB02AD(CAYSQ,SINP,COSP,E,F)
C
      IMPLICITREAL*8(A-H,O-Z)
      PHI=DATAN(SINP/COSP)
      IF(CAYSQ*SINP*SINP-0.5D0)1,1,5
    1 H=1.0D0
      A=PHI
      N=0
      SIG1=0.D0
      SIG2=0.D0
      SIN2=SINP*SINP
      TERM=SINP*COSP*0.5D0
      CRIT=PHI
    2 N=N+1
      RECIP=1.0D0/N
      FACT=(N-.5D0)*RECIP
      H1=H
      H=FACT*CAYSQ*H
      A=FACT*A-TERM*RECIP
      TERM=TERM*SIN2
      CRIT=CRIT*SIN2
      DEL1=H*A
      DEL2=-.5D0*RECIP*CAYSQ*H1*A
      SIG1=SIG1+DEL1
      SIG2=SIG2+DEL2
      IF(DABS(DEL1)-4.0D-16)4,3,3
   3  IF(DABS(CRIT)-DABS(A))4,2,2
    4 F=PHI+SIG1
      E=PHI+SIG2
      GO TO 8
    5 CFI=1.D0
      CFJ=1.D0
      CFL=0.D0
      CFM=0.D0
      CFN=0.D0
      SIG1=0.D0
      SIG2=0.D0
      SIG3=0.D0
      SIG4=0.D0
      N=0
      FACT1=1.0D0-CAYSQ*SINP*SINP
      FACTOR=.5D0*COSP*DSQRT(CAYSQ/FACT1)
      FACTRO=FACTOR+FACTOR
      CAYDSQ=1.0D0-CAYSQ
    6 N=N+1
      RECIP=1.0D0/N
      FACTN=RECIP*(N-.5D0)
      FACTM=(N+.5D0)/(N+1.0D0)
      FACTOR=FACTOR*FACT1
      CFI1=CFI
      CFJ1=CFJ
      CFI=CFI*FACTN
      CFJ=CFJ*FACTN*FACTN*CAYDSQ
      CFL=CFL+.5D0/(N*(N-.5D0))
      CFM=(CFM-FACTOR*RECIP*CFI)*FACTM*FACTM*CAYDSQ
      CFN=(CFN-FACTOR*RECIP*CFI1)*FACTN*FACTM*CAYDSQ
      DEL1=CFM-CFJ*CFL
      DEL2=CFN-(FACTN*CFL-.25D0*RECIP*RECIP)*CAYDSQ     *CFJ1
      DEL3=CFJ
      DEL4=FACTM*CFJ
      SIG1=SIG1+DEL1
      SIG2=SIG2+DEL2
      SIG3=SIG3+DEL3
      SIG4=SIG4+DEL4
      IF(DABS (DEL1)-4.0D-16)7,6,6
    7 CAYMOD=DSQRT(CAYSQ)
      FLOG1=DLOG(4.0D0/(DSQRT(FACT1)+CAYMOD*COSP))
      T1=(1.0D0+SIG3)*FLOG1+FACTRO*DLOG(.5D0+.5D0*CAYMOD*DABS (SINP))
      T2=(.5D0+SIG4)*CAYDSQ*FLOG1+1.0D0-FACTRO*(1.0D0-CAYMOD*DABS(SINP))
      F=T1+SIG1
      E=T2+SIG2
    8 RETURN
      END


      SUBROUTINE FB03AD( GN,CACA,P )
C====== 23/03/72 LAST LIBRARY UPDATE
      IMPLICITREAL*8(A-H,O-Z)
      IF(GN)1,2,2
    1 IF(CACA)3,3,4
    3 P=1.5707963268/DSQRT(1.D0-GN)
      RETURN
    4 STH=DSQRT(-GN/(CACA-GN))
      CTH=DSQRT(1.D0-STH*STH)
      CADA=1.D0-CACA
      CALLFB01AD(CACA,     CAPK,CAPE)
      CALLFB02AD(CADA,STH,CTH,E,F)
      BR=CAPE*F-CAPK*(F-E)
      P=CAPK*CTH*CTH+STH*BR/DSQRT(1.D0-GN)
      RETURN
    2 IF(GN-CACA)10,30,20
   10 STH=DSQRT(GN/CACA)
      CTH=DSQRT(1.D0-STH*STH)
      CALLFB01AD(CACA,     CAPK,CAPE)
      CALLFB02AD(CACA,STH,CTH,E,F)
      BR=CAPK*E-CAPE*F
      P=CAPK+BR*STH/(CTH*DSQRT(1.D0-GN))
      RETURN
   30 CALLFB01AD(CACA,     CAPK,CAPE)
      P=CAPE/(1.D0-CACA)
      RETURN
   20 CADA=1.D0-CACA
      PI=3.1415926536
      STH=DSQRT((1.D0-GN)/CADA)
      CTH=DSQRT(1.D0-STH*STH)
      CALLFB01AD(CACA,     CAPK,CAPE)
      CALLFB02AD(CADA,STH,CTH,E,F)
      BR=PI/2.+CAPK*(F-E)-CAPE*F
      P=CAPK+BR*DSQRT(GN)/(CADA*STH*CTH)
      RETURN
      END
      SUBROUTINE FNMIRK(N,X,H,Y,DY,D,E,BFUN,  NDEX)
      IMPLICIT REAL*8(A-H,O-Z)
      EXTERNAL BFUN
      DIMENSION Y(1),DY(1),D(1),E(1)
      IF( NDEX.NE.0) GO TO 20
      DO 10 I=1,N
      D(I)=Y(I)
   10 CONTINUE
      CALL DERIV ( BFUN )
      HALFH=0.5*H
      RETURN
   20 DO 30 I=1,N
      T=HALFH*DY(I)
      Y(I)=D(I)+T
      E(I)=T
   30 CONTINUE
      XZERO=X
      X=X+HALFH
      CALL DERIV ( BFUN )
      DO 40 I=1,N
      T=HALFH*DY(I)
      Y(I)=D(I)+T
      E(I)=E(I)+2.0*T
   40 CONTINUE
      CALL DERIV ( BFUN )
      DO 50 I=1,N
      T=H*DY(I)
      Y(I)=D(I)+T
      E(I)=E(I)+T
   50 CONTINUE
      X=XZERO+H
      CALL DERIV ( BFUN )
      DO 60 I=1,N
      Y(I)=D(I)+(E(I)+HALFH*DY(I))*.333333333
      D(I)=Y(I)
   60 CONTINUE
      CALL DERIV ( BFUN )
      RETURN
      END
C


C*IBM FUNCTION DASIN(X)
C****
C**** ROUTINE TO PASS CALL TO IBM DOUBLE PRECISION ARC-SINE
C****
C*IBM IMPLICIT REAL*8(A-H,O-Z)
C*IBM DASIN = DARSIN(X)
C*IBM RETURN
C*IBM END


      SUBROUTINE  LENS ( NO,  NP,T, TP ,NUM )
C****
C****
C**** THIN LENS ROUTINE
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      DIMENSION DATA(  75,200 ), ITITLE(200)
C**** DATA  C/ 3.D10/
C****
  100 FORMAT( /  '   THIN LENS     ****  ', A4, '****************',//
     1'      T CM', 18X, 'X CM', 7X, 'Y CM', 7X, 'Z CM' , '      VELZ/C'
     2   , '    THETA MR      PHI MR'  /  )
  103 FORMAT( F10.4, 11X, 3F11.3, F12.5, 2F12.3  )
C****
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 1
      CALL PLT2 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 100) ITITLE(NO)
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
      VZP =  VZA  / VEL
      TP = T*VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      XXA = XA
      YYA = YA
      CS = DATA(9,NO)
      XA =XXA*DATA(1,NO) + VXP*DATA(2,NO)
      VXP =XXA*DATA(3,NO) + VXP*DATA(4,NO) -
     1     CS*DATA(3,NO)**4 * ( XXA*XXA + YYA*YYA )*XXA/10**9
      YA =YYA*DATA(5,NO) + VYP*DATA(6,NO)
      VYP =YYA*DATA(7,NO) + VYP*DATA(8,NO) -
     1     CS*DATA(7,NO)**4 * ( XXA*XXA + YYA*YYA )*YYA/10**9
      VXA = VEL*DSIN( VXP/1000.D0 )
      VYA = VEL*DSIN( VYP/1000.D0 )
      VZA = DSQRT(VEL*VEL -VXA*VXA-VYA*VYA)
      VZP = VZA/VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT2 ( NUM, NO, NBR, TPAR )
      RETURN
      END


      SUBROUTINE NDIP
C****
C****
C**** MTYP = 3 OR 4
C**** THIS VERSION OF BFUN IS MAINLY FOR NONUNIFORM FIELD MAGNETS
C**** THE CENTRAL FIELD REGION IS REPRESENTED TO 3'RD ORDER ON-AND-
C**** OFF THE MIDPLANE BY ANALYTIC EXPRESSIONS. SEE SLAC NO. 75
C**** FRINGE FIELD REGIONS REPRESENTED BY FERMI TYPE FALL-OFF
C**** ALONG WITH RADIAL FALL-OFF
C**** COMPONENTS OF 'B' IN FRINGE REGION EVALUATED BY NUMERICAL METHODS
C****
C****
C**** THE RELATIONSHIP BETWEEN B0, ......... B12 AND B(I,J) RELATIVE TO
C**** AXES (Z,X) IS GIVEN BY
C****
C****
C**** B0  = B( 0, 0 )
C**** B1  = B( 1, 0 )
C**** B2  = B( 2, 0 )
C**** B3  = B( 1, 1 )
C**** B4  = B( 1,-1 )
C**** B5  = B( 0, 1 )
C**** B6  = B( 0, 2 )
C**** B7  = B( 0,-1 )
C**** B8  = B( 0,-2 )
C**** B9  = B(-1, 0 )
C**** B10 = B(-2, 0 )
C**** B11 = B(-1, 1 )
C**** B12 = B(-1,-1 )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  NDX, K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
      X = TC(1)
      Y = TC(2)
      Z = TC(3)
      DX = X - XC
      DZ = Z - ZC
      RP =DSQRT( DX**2 + DZ**2 )
      DR = RP - RB
      GO TO ( 1, 2, 3, 14 ), IN
    7 CONTINUE
c    7 write (6, 8) IN, MTYP
      CALL EXIT
    8 FORMAT (    '0 ERROR -GO TO -  IN BFUN   IN=', I3, '   MTYP=',I4 )
    2 DRR1 = DR/RB
      DRR2 = DRR1*DRR1
      DRR3 = DRR2*DRR1
      DRR4 = DRR3*DRR1
      IF( Y .NE. 0. )  GO TO 4
C****
C**** MID-PLANE UNIFORM FIELD REGION
C****
      BX = 0.
      BY = 0.
      IF( MTYP .EQ. 3) BY=
     1     BF* ( 1. - NDX*DRR1 + BET1*DRR2 + GAMA*DRR3 + DELT*DRR4 )
      IF( MTYP .EQ. 4) BY= BF/ (1. + NDX*DRR1 )
      BZ = 0.
      BT = BY
      RETURN
C****
C**** NON MID-PLANE UNIFORM FIELD REGION
C****
    4 YR1 = Y/RB
      YR2 = YR1*YR1
      YR3 = YR2*YR1
      YR4 = YR3*YR1
      RR1 = RB/RP
      RR2 = RR1*RR1
      RR3 = RR2*RR1
      IF( MTYP .EQ. 3 ) GO TO 11
      IF( MTYP .EQ. 4 ) GO TO 12
      GO TO 7
C****
C**** MTYP = 3
C****
   11 BRR = BF*( ( -NDX + 2.*BET1*DRR1 + 3.*GAMA*DRR2 + 4.*DELT*DRR3 )
     1   *YR1 - (NDX*RR2 + 2.*BET1*RR1*(1.-RR1*DRR1) +
     2   3.*GAMA*( 2. + 2.*RR1*DRR1 - RR2*DRR2 ) +
     3   4.*DELT*( 6.*DRR1 + 3.*RR1*DRR2 - RR2*DRR3 ))*YR3/6. )
      BY = BF* ( 1. - NDX*DRR1 + BET1*DRR2 + GAMA*DRR3 + DELT*DRR4 -
     1   .5*YR2*( -NDX*RR1 + 2.*BET1*( 1. + RR1*DRR1) +
     2   3.*GAMA*DRR1*( 2. + RR1*DRR1) + 4.*DELT*DRR2*(3. + RR1*DRR1) )
     3   + YR4*( -NDX*RR3 + 2.*BET1*( RR3*DRR1 - RR2) +
     4   3.*GAMA*( 4.*RR1 - 2.*RR2*DRR1 + RR3*DRR2 ) +
     5   4.*DELT*( 6. + 12.*RR1*DRR1 - 3.*RR2*DRR2 + RR3*DRR3 ) )/24. )
      GO TO 13
C****
C**** MTYP = 4
C****
   12 DNR1 = 1. + NDX*DRR1
      DNR2 = DNR1*DNR1
      DNR3 = DNR2*DNR1
      DNR4 = DNR3*DNR1
      DNR5 = DNR4*DNR1
      BRR = BF*NDX*( -YR1/DNR2 + YR3*( 6.*NDX*NDX/DNR4 -
     1   2.*NDX*RR1/DNR3 - RR2/DNR2 ) /6.  )
      BY = BF*( 1./DNR1 + .5*YR2*NDX*( -2.*NDX/DNR3 + RR1/DNR2) +
     2   YR4*NDX*( 24.*NDX**3 /DNR5 - 12.*NDX*NDX*RR1/DNR4 -
     3   2.*NDX*RR2/DNR3 - RR3/DNR2 ) /24.  )
C****
C****
   13 BX = BRR*DX/RP
      BZ = BRR*DZ/RP
      BT  =DSQRT(BX*BX + BY*BY + BZ*BZ)
      RETURN
C****
C****
    1 SINE = -1.
      GO TO 5
    3 SINE = 1.
    5 IF( Z  .GT. 0. ) DR = X * SINE*CSC
      CALL NDPP( B0, Z, X, Y, DR      )
      IF( Y  .NE. 0. )  GO TO 6
C****
C**** MID-PLANE FRINGING FIELD REGION
C****
      BX = 0.
      BY = B0
      BZ = 0.
      BT   = B0
      RETURN
C****
C**** NON MID-PLANE FRINGING FIELD REGION
C****
    6 IF( Z .GT. 0. )  GO TO 9
      DR1  =       (DSQRT( DX**2 + (DZ+DG)**2 ) - RB )
      DR2  =       (DSQRT( DX**2 + (DZ+2.*DG)**2 ) - RB )
      DR3  =       (DSQRT( (DX+DG)**2 + (DZ+DG)**2 )  - RB )
      DR4  =       (DSQRT( (DX-DG)**2 + (DZ+DG)**2 )  - RB )
      DR5  =       (DSQRT( (DX+DG)**2 + DZ**2 ) - RB )
      DR6  =       (DSQRT( (DX+ 2.*DG)**2 + DZ**2 ) - RB )
      DR7  =       (DSQRT( (DX-DG)**2 + DZ**2 ) - RB )
      DR8  =       (DSQRT( (DX- 2.*DG)**2 + DZ**2 ) - RB )
      DR9  =       (DSQRT( DX**2 + (DZ-DG)**2 ) - RB )
      DR10 =       (DSQRT( DX**2 + (DZ-2.*DG)**2 ) - RB )
      DR11 =       (DSQRT( (DX+DG)**2 + (DZ-DG)**2 )  - RB )
      DR12 =       (DSQRT( (DX-DG)**2 + (DZ-DG)**2 )  - RB )
      GO TO 10
    9 DR1  = SINE* X*CSC
      DR2  = DR1
      DR9  = DR1
      DR10 = DR1
      DR3  = SINE* ( X + DG )*CSC
      DR5  = DR3
      DR11 = DR3
      DR4  = SINE*( X - DG )*CSC
      DR7  = DR4
      DR12 = DR4
      DR6  = SINE* ( X + 2.*DG )*CSC
      DR8  = SINE* ( X - 2.*DG )*CSC
C****
C****
   10 CALL NDPP ( B1 , Z + DG, X , Y , DR1 )
      CALL NDPP ( B2 , Z + 2.*DG, X , Y , DR2 )
      CALL NDPP ( B3 , Z + DG, X + DG , Y , DR3 )
      CALL NDPP ( B4 , Z + DG, X - DG , Y , DR4 )
      CALL NDPP ( B5 , Z , X + DG , Y, DR5 )
      CALL NDPP ( B6 , Z , X + 2.*DG , Y , DR6 )
      CALL NDPP ( B7 , Z , X - DG , Y, DR7 )
      CALL NDPP ( B8 , Z , X - 2.*DG , Y , DR8 )
      CALL NDPP ( B9 , Z - DG, X , Y , DR9 )
      CALL NDPP ( B10, Z - 2.*DG, X, Y, DR10 )
      CALL NDPP ( B11, Z - DG, X + DG , Y , DR11 )
      CALL NDPP ( B12, Z - DG, X - DG , Y , DR12 )
      YG1 = Y/DG
      YG2 = YG1**2
      YG3 = YG1**3
      YG4 = YG1**4
      BX = YG1 * ( (B5-B7)*2./3. - (B6-B8)/12. )  +
     1     YG3*( (B5-B7)/6. - (B6-B8)/12. -
     2     (B3 + B11 - B4 - B12 - 2.*B5 + 2.*B7 ) / 12. )
      BY = B0 - YG2*( ( B1 + B9 + B5 + B7 - 4.*B0 ) *2./3. -
     1     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. ) +
     2     YG4* (-( B1 + B9 + B5 + B7 - 4.*B0 ) / 6. +
     3     ( B2 + B10 + B6 + B8 - 4.*B0 ) / 24. +
     4     ( B3 + B11 + B4 + B12 - 2.*B1 - 2.*B9 -
     5     2.*B5 - 2.*B7 + 4.*B0 ) / 12. )
      BZ = YG1*( (B1 - B9 ) *2./3. - ( B2 - B10 ) /12. ) +
     1     YG3*( ( B1 - B9 ) / 6. - ( B2 - B10 ) / 12. -
     2     ( B3 + B4 - B11 - B12 - 2.*B1 + 2.*B9 ) / 12.  )
      BT  =DSQRT(BX*BX + BY*BY + BZ*BZ)
      RETURN
   14 BX = 0.
      BY = BR
      BZ = 0.
      BT = BR
      RETURN
      END


      SUBROUTINE  NDPP ( BFLD, Z, X, Y , DR )
C****
C****
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  NDX, K
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK20/  NDX,BET1,GAMA,DELT,CSC
      COMMON  /BLCK21/  RCA,DELS,BR,S2,S3,S4,S5,S6,S7,S8,SCOR
      COMMON  /BLCK22/  D, DG, S, BF, BT
      COMMON  /BLCK23/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK24/  RB, XC, ZC
      COMMON  /BLCK25/  IN, MTYP
      DIMENSION TC(6), DTC(6)
      DRR1 = DR/RB
      DRR2 = DRR1*DRR1
      DRR3 = DRR2*DRR1
      DRR4 = DRR3*DRR1
C****
C**** MTYP    :                  MODIFIED ITERATIVE PROCEDURE
C****
      XP = X
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3 * XP
      ZP = -(S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
     1       S7*XP4*XP3 + S8*XP4*XP4 )
      AZ = (Z-ZP)/10.D0
      AZMAX = DSQRT(  X*X + Z*Z  )
      IF( AZ  .GT.  AZMAX  )  AZ = AZMAX
      ZSIGN = Z-ZP
      RINV4 = 0.
      DO 11 I=1,21
      XP   = X + AZ*(I-11)
      XP2 = XP*XP
      XP3 = XP2*XP
      XP4 = XP3*XP
      ZP = -(S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
     1       S7*XP4*XP3 + S8*XP4*XP4 )
      XXP = X-XP
      ZZP = Z-ZP
      DD =            XXP*XXP + ZZP*ZZP
      IF( DD  .LT.  1.D-15 )  DD = 1.D-15
      IF( DD  .GT.  1.D15  )  DD = 1.D15
      RINV4 = RINV4 + 1.0D0 / (DD*DD )
   11 CONTINUE
      DP = DSQRT( 1.D0/RINV4 )
      DP = DSQRT( DP )
      S = 1.9023D0* DSIGN( 1.D0, ZSIGN ) * DP/D + DELS
C****
C**** FIRST GUESS FOR CLOSEST POINT IS
C****
C*    XP = X
C*    XP2 = XP*XP
C*    XP3 = XP2*XP
C*    XP4 = XP3*XP
C****
C**** CALCULATE ZP ON CURVE FOR CORRESPONDING XP
C****
C*    ZP = -( S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
C*   1   S7*XP4*XP3 + S8*XP4*XP4 )
C*    ZSIGN = Z-ZP
C****
C**** SLOPE OF CURVE AT XP, ZP
C****
C*    DO 4 I=1,3
C*    DZDXC = -(2.*S2*XP + 3.*S3*XP2+ 4.*S4*XP3 + 5.*S5*XP4 +
C*   1   6.*S6*XP4*XP + 7.*S7*XP4*XP2 + 8.*S8*XP4*XP3 )
C****
C**** NEXT APPROXIMATION TO CLOSEST POINT IS
C****
C*    XP = ( DZDXC*(Z-ZP)  +  DZDXC*DZDXC*XP + X ) / (1.+DZDXC*DZDXC)
C*    IF( I  .EQ.  1 )  XP = (3.*XP +  X ) / 4.
C*    XP2 = XP*XP
C*    XP3 = XP2*XP
C*    XP4 = XP3*XP
C*    ZP = -( S2*XP2 + S3*XP3 + S4*XP4 + S5*XP4*XP + S6*XP4*XP2 +
C*   1   S7*XP4*XP3 + S8*XP4*XP4 )
C*  4 CONTINUE
C*    XXP = X-XP
C*    ZZP = Z-ZP
C*    S = DSIGN( 1.D0,ZSIGN) * DSQRT( XXP*XXP + ZZP*ZZP) / D + DELS
C****
C****
C****
C****
      CS=C0+S*(C1+S*(C2+S*(C3+S*(C4+S*C5))))
      IF( DABS(CS)  .GT.  70.  )  CS =DSIGN( 70.D0 ,CS  )
      E=DEXP(CS)
      P0 = 1.0 + E
      DB=BF-BR
      BFLD = 0.
      IF( MTYP .EQ. 3 ) BFLD =
     1       BR +( 1. - NDX*DRR1 + BET1*DRR2+GAMA*DRR3+DELT*DRR4)*DB/P0
      IF( MTYP .EQ. 4 ) BFLD = BR + ( 1./(1. +NDX*DRR1) )*DB/P0
C****
C**** write(6,100) X, Y, Z,  DR, S, BFLD
C*100 FORMAT( 1P6D15.4 )
C****
      RETURN
      END
      SUBROUTINE OPTIC( J, JFOCAL, NP, T, TP )
C****
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 2/  XO, YO, ZO, VXO, VYO, VZO, RTL(1000), RLL(1000)
      COMMON /BLCK 3/ XOR , YOR , ZOR , TH0, PH0, TL1
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
C**** DATA  C/ 3.D10/
C****
C****
  100 FORMAT( /  ' INTERSECTION POINT IN XZ-PLANE OF CENTRAL RAY AND THI
     1S RAY '      )
  101 FORMAT(  ' (IN D AXIS SYSTEM                 )       '         )
  102 FORMAT(  ' (IN OPTIC AXIS SYSTEM             )        '        )
  103 FORMAT( / ' RAY PARAMETERS AT THE FOCAL AXIS SYSTEM  '         )
  104 FORMAT( / ' COORDINATE TRANSFORMATION TO OPTIC AXIS SYSTEM  '  )
C****
C****
C****
  105 FORMAT( / '  *****************************************************
     1************************************************************'/  )
c      IF( NP  .LE. 100) write (6, 105)
      IF( J  .GT.  2  )  GO TO 19
      IF( J  .EQ.  1 )  GO TO 15
      IF( J  .EQ. 2)  GO TO 18
      CALL EXIT
   15 B1X = XA
      B1Y = YA
      S1X = VXA/VZA
      S1Y = VYA/VZA
      TT = T
      VEL1 = VEL
      VZA1 = VZA
      S1XP = DATAN2( VXA,VZA )
      COS1 =DCOS(S1XP)
      SIN1 =DSIN(S1XP)
      ZZZZ = 0.
      TT1 = TT*1.0D+09
      TL1 = TT*VEL
      TH0 = 1000. * S1XP
      PH0 = 1000. * DASIN (VYA/VEL)
      GO TO 17
   18 B2X = XA
      B2Y = YA
      S2X = VXA/VZA
      S2Y = VYA/VZA
C****
C**** CALCULATE CENTRAL AND PARAXIAL RAY INTERCEPTS IN SYSTEM - D
C****
      DSX = S1X-S2X
      IF( DSX .EQ. 0. )   DSX = 1.D-30
      ZINT =  ( B2X-B1X) /  DSX
      XINT = ( B2X*S1X - B1X*S2X ) /  DSX
      YINT = S2Y*ZINT + B2Y
      XOR  = XINT
      YOR  = 0.
      ZOR  = ZINT
      IF( JFOCAL .EQ. 0 ) GO TO 14
      XOR  = B1X
      ZOR  = 0.
   14 CONTINUE
C
C      always print intersection 
C
C      IF( NP  .GT. 100 ) GO TO 5
c      write (6, 100)
c      write (6, 101)
c      write (6, 114) XINT, YINT, ZINT
  114 FORMAT(  14X, 6HXXINT= , F11.4,  3H CM ,  /
     1         14X, 6HYYINT= , F11.4,  3H CM ,  /
     2         14X, 6HZZINT= , F11.4,  3H CM ,  /          )
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
C****
C**** ALTERATION OF INTERCEPTS TO OPTIC AXIS SYSTEM
C****
    5 ZINTZ = ZINT*COS1 + (XINT-B1X) *SIN1
      XINTX =-ZINT*SIN1 + (XINT-B1X) *COS1
      ZZZZ = ZINTZ
      IF( JFOCAL  .NE.  0 )  ZZZZ = 0.
C****
C**** FLIGHT PATH AND TIME FOR RAY-1 IN FOCAL AXIS SYSTEM
C****
      TT = TT + ZZZZ/DABS(VZA1)
      TT1 = TT*1.0D+09
      TL1 = TT*VEL1
C
C      always print intersection 
C
C      IF( NP  .GT. 100 ) GO TO 17
c      write (6, 102)
c      write (6, 114) XINTX, YINT, ZINTZ
      GO TO 17
C****
C**** GENERAL RAY INTERCEPTS IN D-AXIS SYSTEM
C****
   19 BJX = XA
      BJY = YA
      SJX = VXA/VZA
      SJY = VYA/VZA
      DSX = S1X-SJX
      IF( DSX .EQ. 0. )   DSX = 1.D-30
      XINT1 = ( BJX*S1X - B1X*SJX ) /  DSX
      ZINT1 = ( BJX - B1X ) /  DSX
      YINT1 = SJY*ZINT1 + BJY
      IF( NP  .GT. 100 ) GO TO 17
c      write (6, 100)
c      write (6, 101)
c      write (6, 114) XINT1, YINT1, ZINT1
C****
C**** TRANSFORM SYSTEM-D TO OPTIC AXIS SYSTEM
C**** TRANSLATE TO (B1X,0) AND ROTATE BY (S1X,0)
C****
   17 IF( JFOCAL .EQ. 2 ) GO TO 13
      XT = XA
      ZT = ZA
      VXT = VXA
      VZT = VZA
      ZA    = ZT*COS1 + ( XT-B1X ) *SIN1
      XA    =-ZT*SIN1 + ( XT-B1X ) *COS1
      VZA   = VZT*COS1 + VXT*SIN1
      VXA   =-VZT*SIN1 + VXT*COS1
   13 CONTINUE
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. * DASIN( VYA/VEL )
      VZP = VZA   / VEL
      TP = T * VEL
      IF( NP  .GT. 100 ) GO TO 16
c      write (6, 104)
C****
c      write (6, 115) TP, XA,  YA,  ZA,        VZP, VXP, VYP
   16 TDT = -ZA    /DABS( VZA   )
      XA = XA       + TDT * VXA
      YA = YA       + TDT * VYA
      ZA = ZA       + TDT * VZA
      T = T + TDT
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. * DASIN( VYA/VEL )
      VZP = VZA   / VEL
      TP = T * VEL
C****
C**** TRANSLATE PARTICLE TO FOCAL AXIS SYSTEM
C****
      XINT2= XA    + ZZZZ* VXA/VZA
      YINT2= YA    + ZZZZ* VYA/VZA
      ZINT2 = 0.
C****
C****
      TT = T + ZZZZ/DABS(VZA)
      TTJ = TT*1.0D+09
      TLJ = TT*VEL
C****
C**** PATH LENGTHS AND TIMES RELATIVE TO RAY-1
C****
      TTJ1 = TTJ - TT1
      TLJ1 = TLJ - TL1
C****
C****
      XO(J) = XINT2
      YO(J) = YINT2
      ZO(J) = ZA
      VXO(J) = VXP
      VYO(J) = VYP
      VZO(J) = VZP
C****
C**** SAVE TIME DIFFERENCES IN UNITS OF VELOCITY OF RAY-1
C****
      RTL(J) = TTJ1*VEL1*1.0D-09
      RLL(J) = TLJ1
      IF( NP  .GT. 100 ) RETURN
c      write (6, 115) TP, XA,  YA,  ZA,        VZP, VXP, VYP
c      write (6, 103)
c      write (6, 116) XINT2,VXP, YINT2,VYP,ZINT2,TLJ,TLJ1,TTJ,TTJ1
  116 FORMAT( / 20X, 'X=', F10.4, ' CM', 5X, 'VX=',F10.4,' MR',    /
     1          20X, 'Y=', F10.4, ' CM', 5X, 'VY=',F10.4,' MR',    /
     2          20X, 'Z=', F10.4, ' CM'          /
     3          20X, 'L=', F10.4, ' CM', 5X,'DL=',F10.4, ' CM' /
     4          20X, 'T=', F10.4, ' NS', 5X,'DT=',F10.4, ' NS' )
c      IF( JFOCAL  .NE.  0 )  write (6, 99)
   99 FORMAT( / '   FOCAL POS FIXED BY INPUT DATA = IMAGE DISTANCE  '/ )
      RETURN
      END


      SUBROUTINE PLTOUT ( JEN, J, NUM )
C
C THIS ROUTINE STORES STEP-BY-STEP POSITION INFORMATION FOR EACH
C RAY FOR USE BY PLOTTING ROUTINES.
C
C   MODIFIED BY E.A.S.E. TO PRODUCE ASCII OUTPUT FILE IN
C FORMATTED FORM.  THIS ALLOWS EASIER COMPATIBILITY WITH
C RPLOT, REGARDLESS OF WHETHER OUTPUT IS FROM SINGLE OR
C DOUBLE PRECISION VERSION OF RAYTRACE.
C
      IMPLICIT REAL*8 (A-H,O-Z)
      REAL*8 K
      LOGICAL LPLT
      COMMON  /BLCK00/ LPLT
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      DIMENSION TC(6), DTC(6)
      DIMENSION GRAPH(4,512), ICOR(512,2)
C
      IF( NUM .GT. 512 ) NUM = 512
      WRITE (1, 811)   JEN, J, NUM
  811 FORMAT(1X, 3I4)
      DO 100 I = 1, NUM
      WRITE (1,812) GRAPH(1,I), GRAPH(2,I),
     1GRAPH(3,I), GRAPH(4,I), ICOR(I,1), ICOR(I,2)
  812 FORMAT(4(1X, F9.3), 2(1X, I3))
  100 CONTINUE
      RETURN
C
      ENTRY   PLT1( NUM, NO, NBR, TPAR )
C
      IF( .NOT. LPLT ) RETURN
      IF( NUM .GT. 512 ) RETURN
      GRAPH( 1,NUM) = TC(1)
      GRAPH( 2,NUM) = TC(2)
      GRAPH( 3,NUM) = TC(3)
      GRAPH( 4,NUM) = TPAR
      ICOR ( NUM,1) = NO
      ICOR ( NUM,2) = NBR
      RETURN
C
      ENTRY   PLT2( NUM, NO, NBR, TPAR )
C
      IF( .NOT. LPLT ) RETURN
      IF( NUM .GT. 512 ) RETURN
      GRAPH( 1,NUM) = XA
      GRAPH( 2,NUM) = YA
      GRAPH( 3,NUM) = ZA
      GRAPH( 4,NUM) = TPAR
      ICOR ( NUM,1) = NO
      ICOR ( NUM,2) = NBR
      RETURN
      END


      SUBROUTINE POLES  ( NO, NP, T, TP ,NUM )
C****
C****
C**** MULTIPOLE     RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF1, LF2, LU1, K, L
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK90/  D, S, BT, GRAD1,GRAD2,GRAD3,GRAD4,GRAD5
      COMMON  /BLCK91/  C0, C1, C2, C3, C4, C5
      COMMON  /BLCK92/  IN
      COMMON  /BLCK93/  DH, DO, DD, DDD, DSH, DSO, DSD, DSDD
      DIMENSION DATA(  75,200 ), ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
      EXTERNAL  BPOLES
C**** DATA  C/ 3.D10/
C****
      LF1  = DATA(  1,NO )
      LU1  = DATA(  2,NO )
      LF2  = DATA(  3,NO )
      A    = DATA( 10,NO )
      B    = DATA( 11,NO )
      L    = DATA( 12,NO )
      RAD  = DATA( 13,NO )
      BQD  = DATA( 14,NO )
      BHX  = DATA( 15,NO )
      BOC  = DATA( 16,NO )
      BDC  = DATA( 17,NO )
      BDD  = DATA( 18,NO )
      Z11  = DATA( 19,NO )
      Z12  = DATA( 20,NO )
      Z21  = DATA( 21,NO )
      Z22  = DATA( 22,NO )
      FRH  = DATA( 35,NO )
      FRO  = DATA( 36,NO )
      FRD  = DATA( 37,NO )
      FRDD = DATA( 38,NO )
      DSH  = DATA( 39,NO )
      DSO  = DATA( 40,NO )
      DSD  = DATA( 41,NO )
      DSDD = DATA( 42,NO )
      DTF1= LF1/ VEL
      DTF2= LF2/ VEL
      DTU = LU1/ VEL
      D = 2. * RAD
      IF( FRH  .EQ. 0. ) FRH  = 1.D0
      IF( FRO  .EQ. 0. ) FRO  = 1.D0
      IF( FRD  .EQ. 0. ) FRD  = 1.D0
      IF( FRDD .EQ. 0. ) FRDD = 1.D0
      DH  = FRH *D
      DO  = FRO *D
      DD  = FRD *D
      DDD = FRDD*D
      GRAD1 = -BQD/RAD
      GRAD2 =  BHX/RAD**2
      GRAD3 = -BOC/RAD**3
      GRAD4 =  BDC/RAD**4
      GRAD5 = -BDD/RAD**5
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S = 0.
C****
      IF( NP  .GT. 100 ) GO TO 5
c      write (6, 100) ITITLE(NO)
  100 FORMAT(  ' MULTIPOLE(POLES)  ****  ', A4,'  ******************'/)
C****
c      write (6, 101)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HBX, 8X, 4HY CM , 7X, 2HBY,
     1   8X, 4HZ CM, 7X, 2HBZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HB             )
      CALL PRNT2 ( T,S,XA   ,YA   ,ZA   ,BX,BY,BZ,BT,VXA  ,VYA  ,VZA   )
c      write (6, 103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO B AXIS SYSTEM '       )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO VFB COORD.
C****
    5 TC(1) = -XA
      TC(2) = YA
      TC(3) = A - ZA
      TC(4) = -VXA
      TC(5) = VYA
      TC(6) = -VZA
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FIRST FRINGE FIELD
C****
      TDT = ( TC(3) - Z11 ) /DABS( TC(6) )
C****
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** IN DESIGNATES FIELD REGIONS FOR MULTIPOLE
C****
      IN = 1
      C0   = DATA( 23,NO )
      C1   = DATA( 24,NO )
      C2   = DATA( 25,NO )
      C3   = DATA( 26,NO )
      C4   = DATA( 27,NO )
      C5   = DATA( 28,NO )
c      IF( NP  .LE. 100) write (6, 104)
  104 FORMAT( 22H0FRINGING FIELD REGION    )
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BPOLES,0    )
      NSTEP = 0
    6 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BPOLES,1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( Z12 .GE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 =-( Z12 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BPOLES, 0    )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BPOLES, 1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
  105 FORMAT( 10H   NSTEPS= ,I5 )
C***
C***  UNIFORM FIELD REGION
C**** TRANSFORM TO SECOND VFB COORD SYSTEM
C***
      GRAD1 = -GRAD1
      GRAD2 =  GRAD2
      GRAD3 = -GRAD3
      GRAD4 =  GRAD4
      GRAD5 = -GRAD5
      TC(1) = -TC(1)
      TC(3) = -TC(3) - L
      TC(4) = -TC(4)
      TC(6) = -TC(6)
C****
C****
C**** UNIFORM FIELD INTEGRATION REGION
C****
C****
      IN = 2
c      IF( NP  .LE. 100) write (6, 106)
  106 FORMAT(   '0UNIFORM FIELD REGION IN C AXIS SYSTEM '  )
      IF( TC(3)  .LT.  Z21 ) GO TO 15
C****
C**** THIS SECTION CORRECTS FOR MAGNETS WHOSE FRINGING FIELDS INTERSECT
C****
c      IF( NP  .LE. 100) write (6, 102)
  102 FORMAT( / '   INTEGRATE BACKWARDS    '  )
      CALL FNMIRK( 6, T,-DTU ,TC, DTC, DS, ES,BPOLES, 0    )
      NSTEP = 0
   16 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 17  I =1, NP
      CALL FNMIRK( 6, T,-DTU, TC, DTC, DS, ES,BPOLES, 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .LE.  Z21 )  GO TO 18
   17 CONTINUE
      GO TO 16
   18 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BPOLES, 0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BPOLES, 1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
c      IF( NP  .LE. 100) write (6, 107)
  107 FORMAT( / )
      GO TO 19
C****
C****
   15 CONTINUE
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, BPOLES,0    )
      NSTEP = 0
    9 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 10  I =1, NP
      CALL FNMIRK( 6, T, DTU ,TC, DTC, DS, ES, BPOLES,1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3)  .GE.  Z21 )  GO TO 11
   10 CONTINUE
      GO TO 9
   11 CONTINUE
      XDTU  = ( Z21 - TC(3) ) /DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BPOLES, 0    )
      CALL FNMIRK( 6, T,XDTU ,TC, DTC, DS, ES,BPOLES, 1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
   19 CONTINUE
C***
C***
C**** SETUP FOR SECOND FRINGE FIELD AND INTEGRATION
C****
C****
      C0   = DATA( 29,NO )
      C1   = DATA( 30,NO )
      C2   = DATA( 31,NO )
      C3   = DATA( 32,NO )
      C4   = DATA( 33,NO )
      C5   = DATA( 34,NO )
      IN = 3
c      IF( NP  .LE. 100) write (6, 104)
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BPOLES,0    )
      NSTEP = 0
   12 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 13  I =1, NP
      CALL FNMIRK( 6, T, DTF2,TC, DTC, DS, ES, BPOLES,1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( TC(3) .GE. Z22 )  GO TO 14
   13 CONTINUE
      GO TO 12
   14 CONTINUE
      XDTF2 = ( Z22 - TC(3) ) / TC(6)
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BPOLES,0    )
      CALL FNMIRK( 6, T,XDTF2,TC, DTC, DS, ES, BPOLES,1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      TC(3) = TC(3) - B
c      IF( NP  .LE. 100) write (6, 109)
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
      TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF(NP.LE.100) write (6,115)TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C**
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
C****
C****
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF( NP  .LE. 100) write (6, 111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=, F10.4, 3H MR   /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y= ,F10.2, 3H CM   /   )
      RETURN
   99 CALL PRNT4 (NO, IN)
      RETURN
      END


      SUBROUTINE  PRNT( J,NO )
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 0/  DATA, ITITLE
      COMMON  /BLCK 1/  XI, YI, ZI, VXI, VYI, VZI, DELP
      COMMON  /BLCK 2/  XO, YO, ZO, VXO, VYO, VZO,RTL(1000),RLL(1000)
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 6/  NP, JFOCAL
      COMMON  /DY1/  BDIPOLE
      DIMENSION DATA(  75,200 ), ITITLE(200)
      DIMENSION XO(1000),YO(1000),ZO(1000),VXO(1000),VYO(1000),VZO(1000)
      DIMENSION XI(1000),YI(1000),ZI(1000),VXI(1000),VYI(1000),
     1      VZI(1000),DELP(1000)
      CHARACTER*8 LX(14)
      CHARACTER*8 LCM
      INTEGER ID2(52), ID3(21), ID4(43), ID5(33), ID6(17),ID7(7),ID8(26)
      DATA ID2 / 11, 19, 29, 41, 51, 12, 20, 30, 42, 52, 13, 21, 31,
     1   43, 53, 14, 22, 32, 44, 54, 15, 25, 33, 45, 55, 16, 26, 34,
     2   46, 56, 17, 27, 35, 47, 57, 18, 28, 36, 48, 58, 37, 49, 59, 38,
     3 50,60,39, 61, 40, 62, 63, 64                                  /
      DATA ID3 / 10, 15, 19, 25, 11, 16, 20, 26, 12, 17, 21, 27, 13,
     1   18, 22, 28, 14, 23, 29, 24, 30                              /
      DATA ID4 /  7, 20, 28, 34,  8, 21, 29, 35,  9, 22, 30, 36, 10,
     1   23, 31, 37, 11, 24, 32, 38, 12, 25, 33, 39, 13, 26, 40, 46,
     2   16, 27, 41, 47, 17, 42, 48, 18, 43, 49, 19, 44, 50, 45, 51  /
      DATA ID5 / 10, 14, 19, 23, 29, 11, 15, 20, 24, 30, 12, 16, 21,
     1   25, 31, 13, 17, 22, 26, 32, 18, 27, 33, 28, 34, 35, 39, 36,
     2   40, 37, 41, 38, 42  /
      DATA ID6 / 10, 16, 20, 26, 11, 17, 21, 27, 12, 22, 28, 13, 23,
     1   14, 24, 15, 25                                              /
      DATA ID7 / 10, 15, 11, 16, 12, 13, 14                          /
      DATA ID8 / 11, 16, 25, 29, 35, 12, 17, 26, 30, 36, 13, 18, 27,
     1   31, 37, 14, 19, 28, 32, 38, 15, 20, 33, 39, 34, 40          /
      DATA LCM / ' CM ' /
      DATA LX/ ' ENTR FL','D STEP =',' UNIF FL','D STEP =',
     1         ' EXIT FL','D STEP =',' DIFF/MI','D STEP =',
     2         '        ','   RHO =','        ','  MTYP =',
     3         '   FIELD','  STEP ='                                 /
C****
C****
      GO TO ( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ), J
c      write (6, 109) J
  109 FORMAT(// ' GO TO FELL THROUGH IN ROUTINE PRNT  J= ',I5, ///   )
      CALL EXIT
C****
    1 RETURN
C****
C**** COLLIMATOR DATA
C****
  103 FORMAT( // 20X, '*** COLLIMATOR       ***',   A4  /  )
  104 FORMAT(
     1   5X,'ELPS=',  F9.1, 5X,'XCEN=',  F9.4, 5X,'YCEN=',  F9.4,
     2   5X,'XMAX=',  F9.4, 5X,'YMAX=',  F9.4                       )
   13 CONTINUE
c   13 write (6, 103) ITITLE(NO)
c      write (6, 104) (DATA(I,NO),I=1,5)
      RETURN
C****
C**** DIPOLE DATA
C****
  100 FORMAT( // 20X, '*** DIPOLE MAGNET    ***',   A4  /  )
  101 FORMAT(
     1   5X,'  A =',  F9.4, 5X,'NDX =',  F9.4, 5X,'C01 =',  F9.4,
     2   5X,'BR1 =',  F9.4, 5X,'S02 =',1PE12.3,5X,       2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 5X,'BET1=',  F9.4, 5X,'C02 =',  F9.4,
     4   5X,'BR2 =',  F9.4, 5X,'S03 =',1PE12.3,5X,       2A8,0PF8.3,A4,/
     5   5X,'  D =',  F9.4, 5X,'GAMA=',  F9.4, 5X,'C03 =',  F9.4,
     6   5X,'XCR1=',  F9.4, 5X,'S04 =',1PE12.3,5X,       2A8,0PF8.3,A4,/
     7   5X,'  R =',  F9.4, 5X,'DELT=',  F9.4, 5X,'C04 =',  F9.4,
     8   5X,'XCR2=',  F9.4, 5X,'S05 =',1PE12.3,5X,       2A8,0PF8.3,A4,/
     9   5X,' BF =',  F9.4, 5X,'Z11 =',  F9.4, 5X,'C05 =',  F9.4,
     A   5X,'DLS1=',  F9.4, 5X,'S06 =',1PE12.3,5X,       2A8,  I4     ,/
     B   5X,'PHI =',0PF9.4, 5X,'Z12 =',  F9.4, 5X,'C06 =',  F9.4,
     C   5X,'DLS2=',  F9.4, 5X,'S07 =',1PE12.3,5X,       2A8,0PF8.3,A4 )
  102 FORMAT(
     1   5X,'ALPH=',  F9.4,   5X,'Z21 =',  F9.4,  5X,'C11 =',  F9.4,
     2   5X,'RAP1=',  F9.4,   5X,'S08 =',1PE12.3,/, 5X,'BETA=',0PF9.4,
     3   5X,'Z22 =',  F9.4,   5X,'C12 =',  F9.4,  5X,'RAP2=',  F9.4,
     4   5X,'S12 =',1PE12.3,/,43X,'C13 =',0PF9.4,
     X                        5X,'SCR1=',  F9.4,  5X,'S13 =', 1PE12.3
     5   ,/,43X,'C14 =',0PF9.4,   5X,'SCR2=',  F9.4,
     Y                        5X,'S14 =',1PE12.3,/,43X,'C15 =',0PF9.4,
     6   24X,'S15 =',1PE12.3,/,43X,'C16 =',0PF9.4,24X,'S16 =',1PE12.3
     7  ,/,81X,'S17 =',1PE12.3,/,81X,'S18 =',1PE12.3                )
C****
C****
    2 RHO = 1.D30
      IF( DATA(15,NO) .NE. 0 )
     1RHO = DSQRT( (2.*931.48*PMASS+ENERGY)*ENERGY)/(3.*DATA(15,NO)*Q0)
      MTYP = DATA(5,NO)
c      write (6, 100) ITITLE(NO)
c      write (6,101) (DATA(ID2(I),NO),I=1,5),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID2(I),NO),I= 6,10),LX( 3),LX( 4),DATA(2,NO),LCM,
c     2          (DATA(ID2(I),NO),I=11,15),LX( 5),LX( 6),DATA(3,NO),LCM,
c     3          (DATA(ID2(I),NO),I=16,20),LX( 7),LX( 8),DATA(4,NO),LCM,
c     4          (DATA(ID2(I),NO),I=21,25),LX(11),LX(12),MTYP      ,
c     5          (DATA(ID2(I),NO),I=26,30),LX( 9),LX(10),RHO,       LCM
c      write (6, 102) (DATA(ID2(I),NO),I=31,52)
      RETURN
C****
C**** QUADRUPOLE, HEXAPOLE, OCTAPOLE, DECAPOLE DATA
C****
  200 FORMAT( // 20X, '*** QUADRUPOLE       ***',   A4  /  )
  400 FORMAT( // 20X, '*** SEXTUPOLE        ***',   A4  /  )
  500 FORMAT( // 20X, '*** OCTUPOLE         ***',   A4  /  )
  600 FORMAT( // 20X, '*** DECAPOLE         ***',   A4  /  )
  120 FORMAT(
     1   5X,'  A =',  F9.4, 5X,'Z11 =',  F9.4, 5X,'C01 =',  F9.4,
     2   5X,'C11 =',  F9.4, 5X, 2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 5X,'Z12 =',  F9.4, 5X,'C02 =',  F9.4,
     4   5X,'C12 =',  F9.4, 5X, 2A8,0PF8.3,A4,/
     5   5X,'  L =',  F9.4, 5X,'Z21 =',  F9.4, 5X,'C03 =',  F9.4,
     6   5X,'C13 =',  F9.4, 5X, 2A8,0PF8.3,A4,/
     7   5X,'RAD =',  F9.4, 5X,'Z22 =',  F9.4, 5X,'C04 =',  F9.4,
     8   5X,'C14 =',  F9.4,/5X,' BF =',  F9.4,24X,'C05 =',  F9.4,
     9   5X,'C15 =',  F9.4,/                  43X,'C06 =',  F9.4,
     A   5X,'C16 =',  F9.4                                           )
C****
C****
    3 CONTINUE
c    3 write (6, 200) ITITLE(NO)
      GO TO 21
    4 CONTINUE
c    4 write (6, 400) ITITLE(NO)
      GO TO 21
    5 CONTINUE
c    5 write (6, 500) ITITLE(NO)
      GO TO 21
    6 CONTINUE
c    6 write (6, 600) ITITLE(NO)
   21 CONTINUE
c   21 write (6,120)(DATA(ID3(I),NO),I=1,4),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID3(I),NO),I= 5,8 ),LX( 3),LX( 4),DATA(2,NO),LCM,
c     2          (DATA(ID3(I),NO),I= 9,12),LX( 5),LX( 6),DATA(3,NO),LCM,
c     3          (DATA(ID3(I),NO),I=13,21)
      RETURN
C****
C**** ELECTROSTATIC DEFLECTOR DATA
C****
  190 FORMAT( // 20X, '*** ELECTROSTATIC DEF.***',   A4  /  )
  191 FORMAT(
     1   5X,'  A =',  F9.4, 5X,'PHI =',  F9.4, 5X,'Z11 =',  F9.4,
     2   5X,'C01 =',  F9.4, 5X,'C11 =',  F9.4, 5X,       2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 5X,'EC2 =',  F9.4, 5X,'Z12 =',  F9.4,
     4   5X,'C02 =',  F9.4, 5X,'C12 =',  F9.4, 5X,       2A8,0PF8.3,A4,/
     5   5X,'  D =',  F9.4, 5X,'EC4 =',  F9.4, 5X,'Z21 =',  F9.4,
     6   5X,'C03 =',  F9.4, 5X,'C13 =',  F9.4, 5X,       2A8,0PF8.3,A4,/
     7   5X,'  R =',  F9.4, 5X,'WE  =',  F9.4, 5X,'Z22 =',  F9.4,
     8   5X,'C04 =',  F9.4, 5X,'C14 =',  F9.4, 5X,       2A8,0PF8.3,A4,/
     9   5X,' EF =',  F9.4, 5X,'WC  =',  F9.4,24X,'C05 =',  F9.4,
     A   5X,'C15 =',  F9.4,                    5X,       2A8,0PF8.3,A4,/
     B  62X,'C06 =',0PF9.4, 5X,'C16 =',  F9.4                   )
C****
C****
    7 RHO = 1.D30
      EMASS = PMASS * 931.48
      ETOT = EMASS + ENERGY
      VC2 = (2.*EMASS + ENERGY)*ENERGY / (ETOT*ETOT)
      GAMMA = 1. / DSQRT(1. - VC2)
      IF( DATA(15,NO) .NE. 0 )
     1RHO = GAMMA * EMASS * VC2 * 1000. / (DATA(15,NO) * Q0)
c      write (6, 190) ITITLE(NO)
c      write (6,191)(DATA(ID8(I),NO),I=1,5),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID8(I),NO),I= 6,10),LX( 3),LX( 4),DATA(2,NO),LCM,
c     2          (DATA(ID8(I),NO),I=11,15),LX( 5),LX( 6),DATA(3,NO),LCM,
c     3          (DATA(ID8(I),NO),I=16,20),LX( 7),LX( 8),DATA(4,NO),LCM,
c     4          (DATA(ID8(I),NO),I=21,24),LX( 9),LX(10),RHO,LCM   ,
c     5          (DATA(ID8(I),NO),I=25,26)
      RETURN
C****
C****    VELOCITY SELECTOR DATA
C****
  132 FORMAT( // 20X, '*** VELOCITY SELECTOR***',   A4  /  )
  130 FORMAT(
     1   5X,'  A =',  F9.4, 5X,'Z11 =',  F9.4, 5X,'CB00=',  F9.4,
     2   5X,'CE00=',  F9.4, 5X, 2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 5X,'Z12 =',  F9.4, 5X,'CB01=',  F9.4,
     4   5X,'CE01=',  F9.4, 5X, 2A8,0PF8.3,A4,/
     5   5X,'  L =',  F9.4, 5X,'Z21 =',  F9.4, 5X,'CB02=',  F9.4,
     6   5X,'CE02=',  F9.4, 5X, 2A8,0PF8.3,A4,/
     7   5X,' BF =',  F9.4, 5X,'Z22 =',  F9.4, 5X,'CB03=',  F9.4,
     8   5X,'CE03=',  F9.4, 5X, 2A8,0PF8.3,A4,/
     9   5X,' BE =',  F9.4, 5X,'CB2 =',  F9.4, 5X,'CB04=',  F9.4,
     A   5X,'CE04=',  F9.4, 5X, 2A8,0PF8.3,A4                        )
  131 FORMAT(
     1   5X,' RB =',  F9.4, 5X,'CB4 =',  F9.4, 5X,'CB05=',  F9.4,
     2   5X,'CE05=',  F9.4,/5X,'NDX =',  F9.4, 5X,'CE2 =',  F9.4,
     3   5X,'CB10=',  F9.4, 5X,'CE10=',  F9.4,/5X,' DB =',  F9.4,
     4   5X,'CE4 =',  F9.4, 5X,'CB11=',  F9.4, 5X,'CE11=',  F9.4,/
     5   5X,' DE =',  F9.4,24X,'CB12=',  F9.4, 5X,'CE12=',  F9.4,/
     6   5X,' WB =',  F9.4,24X,'CB13=',  F9.4, 5X,'CE13=',  F9.4,/
     7   5X,' WE =',  F9.4,24X,'CB14=',  F9.4, 5X,'CE14=',  F9.4,/
     8  43X,'CB15=',  F9.4, 5X,'CE15=',  F9.4                        )
C****
C****
    8 RHO = 1.D30
      IF( DATA(10,NO)  .NE.  0.   )
     1RHO = DSQRT( (2.*931.48*PMASS+ENERGY)*ENERGY)/(3.*DATA(10,NO)*Q0)
c      write (6, 132)ITITLE(NO)
c      write (6,130)(DATA(ID4(I),NO),I=1,4),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID4(I),NO),I= 5,8 ),LX( 3),LX( 4),DATA(2,NO),LCM,
c     2          (DATA(ID4(I),NO),I= 9,12),LX( 5),LX( 6),DATA(3,NO),LCM,
c     3          (DATA(ID4(I),NO),I=13,16),LX( 7),LX( 8),DATA(4,NO),LCM,
c     4          (DATA(ID4(I),NO),I=17,20),LX( 9),LX(10),RHO,LCM
c      write (6, 131) (DATA(ID4(I),NO),I=21,43)
      RETURN
C****
C**** MULTIPOLE (POLES)      DATA
C****
  141 FORMAT( // 20X, '*** MULTIPOLES       ***',   A4  /  )
  140 FORMAT(
     1   5X,'  A =',  F9.4, 3X,'BQUAD =',F9.4, 5X,'Z11 =',  F9.4,
     2   5X,'C01 =',  F9.4, 5X,'C11 =',  F9.4, 8X, 2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 3X,'BHEX  =',F9.4, 5X,'Z12 =',  F9.4,
     4   5X,'C02 =',  F9.4, 5X,'C12 =',  F9.4, 8X, 2A8,0PF8.3,A4,/
     5   5X,'  L =',  F9.4, 3X,'BOCT  =',F9.4, 5X,'Z21 =',  F9.4,
     6   5X,'C03 =',  F9.4, 5X,'C13 =',  F9.4, 8X, 2A8,0PF8.3,A4,/
     7   5X,'RAD =',  F9.4, 3X,'BDEC  =',F9.4, 5X,'Z22 =',  F9.4,
     8   5X,'C04 =',  F9.4, 5X,'C14 =',  F9.4,/
     9                     22X,'BDDEC =',F9.4,24X,'C05 =',  F9.4,
     A   5X,'C15 =',  F9.4/62X,'C06 =',  F9.4, 5X,'C16 =',  F9.4
     B                    /62X,'FRH =',  F9.4, 5X,'DSH =',  F9.4
     C                    /62X,'FRO =',  F9.4, 5X,'DSO =',  F9.4
     D                    /62X,'FRD =',  F9.4, 5X,'DSD =',  F9.4
     E                    /62X,'FRDD=',  F9.4, 5X,'DSDD=',  F9.4     )
C****
C****
    9 CONTINUE
c    9 write (6, 141) ITITLE(NO)
c      write (6,140)(DATA(ID5(I),NO),I=1,5),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID5(I),NO),I= 6,10),LX( 3),LX( 4),DATA(2,NO),LCM,
c     2          (DATA(ID5(I),NO),I=11,15),LX( 5),LX( 6),DATA(3,NO),LCM,
c     3          (DATA(ID5(I),NO),I=16,33)
      RETURN
C****
C**** MULTIPOLE DATA
C****
  151 FORMAT( // 20X,  '***MULTIPOLE(HE)    ***',   A4  /  )
  150 FORMAT(
     1   5X,'  A =',  F9.4, 5X,' Z1 =',  F9.4, 5X,' C0 =',  F9.4,
     2   5X,' C6 =',  F9.4, 5X, 2A8,0PF8.3,A4,/
     3   5X,'  B =',  F9.4, 5X,' Z2 =',  F9.4, 5X,' C1 =',  F9.4,
     4   5X,' C7 =',  F9.4, 5X, 2A8,0PF8.3,A4,/
     5   5X,'  L =',  F9.4,24X,' C2 =',  F9.4, 5X,' C8 =',  F9.4/
     6   5X,'  W =',  F9.4,24X,' C3 =',  F9.4,/
     7   5X,'  D =',  F9.4,24X,' C4 =',  F9.4,/
     8   5X,' BF =',  F9.4,24X,' C5 =',  F9.4                        )
C****
C****
   10 CONTINUE
c   10 write (6, 151) ITITLE(NO)
c      write (6,150)(DATA(ID6(I),NO),I=1,4),LX(1),LX(2),DATA(1,NO),LCM,
c     1          (DATA(ID6(I),NO),I= 5,8 ),LX( 7),LX( 8),DATA(2,NO),LCM,
c     2          (DATA(ID6(I),NO),I= 9,17)
      RETURN
C****
C**** TRANSLATE - ROTATE DATA
C****
  170 FORMAT( // 20X, '*** TRANSLATE-ROTATE ***',   A4  /  )
  171 FORMAT(   5X, 5H X0 =,F9.4,        5X,5H Y0 =,F9.4,
     1          5X, 5H Z0 =,F9.4,     /,1X,9HTHETA X =,F9.4,
     2        1X,9HTHETA Y =,F9.4,       1X,9HTHETA Z =,F9.4 )
C****
C****
   11 CONTINUE
c   11 write (6, 170) ITITLE(NO)
c      write (6, 171) ( DATA(I,NO) , I=1,6 )
      RETURN
C****
C**** DRIFT SECTION DATA
C****
   12 CONTINUE
c   12 write (6, 175) ITITLE(NO)
c      write (6, 176) ( DATA(I,NO) , I=1,1 )
  175 FORMAT( // 20X, '*** DRIFT            ***',   A4  /  )
  176 FORMAT(  19X,  ' Z-DRIFT ='  , F9.4,  ' CM'        )
      RETURN
C****
C**** SOLENOID DATA
C****
  161 FORMAT( // 20X, '*** SOLENOID         ***',   A4  /  )
  160 FORMAT(
     1   5X,'  A =',  F9.4, 5X,'Z11 =',  F9.4, 5X,2A8,0PF8.3,A4,/
     2   5X,'  B =',  F9.4, 5X,'Z22 =',  F9.4,/5X,'  L =',  F9.4,/
     3   5X,'DIA =',  F9.4,/5X,' BF =',  F9.4                        )
C****
C****
   14 CONTINUE
c   14 write (6, 161) ITITLE(NO)
c      write (6,160)(DATA(ID7(I),NO),I=1,2),LX(13),LX(14),DATA(1,NO),
c     1         LCM, (DATA(ID7(I),NO),I= 3, 7)
      RETURN
C****
C**** LENS DATA
C****
  180 FORMAT( // 20X, '*** LENS             ***',   A4  /  )
  181 FORMAT(  3X, 7H(X/X) =  ,F9.4,   6H CM/CM,
     1        16X, 7H(X/T) =  ,F9.4,   6H CM/MR, /,
     2         3X, 7H(T/X) =  ,F9.4,   6H MR/CM,
     3        16X, 7H(T/T) =  ,F9.4,   6H MR/MR, /,
     4         3X, 7H(Y/Y) =  ,F9.4,   6H CM/CM,
     5        16X, 7H(Y/P) =  ,F9.4,   6H CM/MR, /,
     6         3X, 7H(P/Y) =  ,F9.4,   6H MR/CM,
     7        16X, 7H(P/P) =  ,F9.4,   6H MR/MR, //,
     8         3X, 7H   CS =  ,F9.4,   6H CM   , /         )
C****
C****
   15 CONTINUE
c   15 write (6, 180) ITITLE(NO)
c      write (6, 181) ( DATA(I,NO) , I=1,9 )
      RETURN
C****
C****
      ENTRY PRNT1 ( N )
C****
C****
c      IF( JFOCAL .EQ. 0 ) write (6, 105)
c      IF( JFOCAL .EQ. 1 ) write (6, 106)
c      IF( JFOCAL .EQ. 2 ) write (6, 107)
c      IF( JFOCAL .GT. 2 ) write (6, 108)
c      write (6, 110)
  105 FORMAT( 1H1, 15X, '****COORDINATES OPTIC AXIS SYSTEM****
     1  ( ORIGIN AT RAY 1-2 INTERSECTION ) '    // )
  106 FORMAT( 1H1, 15X, '****COORDINATES OPTIC AXIS SYSTEM****
     1  ( ORIGIN AT ZD=0.0  ) '    // )
  107 FORMAT( 1H1, 15X, '****COORDINATES D-AXIS SYSTEM****' // )
  108 FORMAT( 1H1, 15X, '****COORDINATES OPTIC AXIS SYSTEM****' // )
  110 FORMAT(
     1   10X, 45HX     THETA     Y      PHI      ZI    DELE     ,5X,
     2   12HXO        XS  , 11X, 12HYO        YS , 6X, 'L(CM)', 5X,
     3   'T(NS)'                         /)
      DO 20  I=1,N
C****
C**** CALCULATE TIME IN (NS)
C****
      TLJ1 = RTL(I)*1.0D+09 / VEL
C      write (6, 111)  I, XI(I), VXI(I), YI(I), VYI(I), ZI(I), DELP(I),
C     1    XO(I), VXO(I), YO(I), VYO(I), RLL(I), TLJ1
  111 FORMAT(       I5,    6F8.2, 2X,  F10.4,   F10.4, 2X,   F10.4,
     1     F10.4 ,   F10.3, F10.3               /)
   20 CONTINUE
c      open(38,file='rayout.plt',status='unknown')
c      do 2100  i=1,n
c      write (38, 582)  I, XI(I), VXI(I), YI(I), VYI(I), ZI(I), DELP(I),
c     1    XO(I), VXO(I), YO(I), VYO(I), RLL(I), TLJ1, BDIPOLE
  582 format(i3,3x,13(f10.5,1x))
 2100 continue
c      write(38,583)
  583 format(1x,'/')
c      close(38)
      RETURN
C****
C****
      ENTRY PRNT2 ( T, S, X, Y, Z, BX, BY, BZ, BT, VX, VY, VZ          )
C****
      IF( NP  .GT. 100 ) RETURN
      VXP = 1000. *DATAN2( VX ,VZ  )
      VYP = 1000. * DASIN( VY /VEL )
      VZP = VZ  / VEL
      TP = T * VEL
c      write (6, 112)TP,S,X, BX, Y, BY, Z, BZ, VZP, VXP, VYP, BT
  112 FORMAT(2F10.4,     F10.3, F11.4, F10.3, F11.4, F10.3, F11.4,
     1        F13.5, F13.2, F11.2, F10.4         )
      RETURN
C****
      ENTRY PRNT3 (TDIST,X,Y,Z,BX,BY,BZ,EX,EY,EZ,VX,VY,VZ)
C****
  114 FORMAT( 2F9.3, 2F10.4,F9.3, 2F10.4,F9.3, 2F10.4,2F11.3, -9PF9.5 )
C****
C****
      IF( NP  .GT. 100 ) RETURN
      VXP = 1000. *DATAN2( VX ,VZ  )
      VYP = 1000. * DASIN( VY /VEL )
      VZP = VZ  / VEL
      TP = T * VEL
c      write (6, 114) TDIST,X,BX,EX,Y,BY,EY,Z,BZ,EZ,VXP,VYP,VEL
      RETURN
C****
C****
C****
        ENTRY  PRNT4(NO, IN)
C****
115     FORMAT (///, 10X, 'MAXIMUM STEPS EXCEEDED', /10X,
     1   'ELEMENT = ', I4, /10X, 'REGION = ', I4 ///)
c        write (6, 115) NO, IN
        RETURN
C****
C****
      ENTRY PRNT5 ( T, S, X, Y, Z, EX, EY, EZ, ET, VX, VY, VZ          )
C****
      IF( NP  .GT. 100 ) RETURN
      VXP = 1000. *DATAN2( VX ,VZ  )
      VYP = 1000. * DASIN( VY /VEL )
      VZP = VZ  / VEL
      TP = T * VEL
c      write (6, 112)TP,S,X, EX, Y, EY, Z, EZ, VZP, VXP, VYP, ET
      RETURN
      END


      SUBROUTINE SHROT  ( NO, NP, T, TP ,NUM )
C****
C****
C**** SUBROUTINE DOES TRANSLATIONS FIRST ALONG AXES X, Y, Z IN ORDER,
C**** FOLLOWED BY ROTATIONS ABOUT X, Y, Z   .
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      DIMENSION DATA(  75,200 ), ITITLE(200)
C**** DATA  C/ 3.D10/
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 1
      CALL PLT2 ( NUM, NO, NBR, TPAR )
      X0 = DATA( 1,NO )
      Y0 = DATA( 2,NO )
      Z0 = DATA( 3,NO )
      CX = DCOS( DATA(4,NO)/57.29578 )
      SX = DSIN( DATA(4,NO)/57.29578 )
      CY = DCOS( DATA(5,NO)/57.29578 )
      SY = DSIN( DATA(5,NO)/57.29578 )
      CZ = DCOS( DATA(6,NO)/57.29578 )
      SZ = DSIN( DATA(6,NO)/57.29578 )
  100 FORMAT( / '   TRANSLATE-ROTATE  ****  ', A4,'  ***************'//
     1'      T CM', 18X, 'X CM', 7X, 'Y CM', 7X, 'Z CM' , '      VELZ/C'
     2   , '    THETA MR      PHI MR'  /  )
  101 FORMAT( '  TRANSLATE  ' )
  102 FORMAT( '  ROTATE  '  )
  103 FORMAT( F10.4, 11X, 3F11.3, F12.5, 2F12.3  )
c      IF( NP  .LE. 100) write (6, 100)
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
      VZP =  VZA  / VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      IF( (X0 .EQ. 0.) .AND. (Y0 .EQ. 0.) .AND. (Z0 .EQ. 0.) ) GO TO 1
c      IF( NP  .LE. 100) write (6, 101)
      XA = XA-X0
      YA = YA-Y0
      ZA = ZA-Z0
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
    1 IF( DATA( 4,NO ) .EQ. 0. ) GO TO 2
c      IF( NP  .LE. 100) write (6, 102)
      YR =  YA*CX +  ZA*SX
      ZR = -YA*SX +  ZA*CX
      VYR= VYA*CX + VZA*SX
      VZR=-VYA*SX + VZA*CX
      YA = YR
      ZA = ZR
      VYA = VYR
      VZA = VZR
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
      VZP =  VZA  / VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
    2 IF( DATA( 5,NO ) .EQ. 0. ) GO TO 3
c      IF( NP  .LE. 100) write (6, 102)
      XR = -ZA*SY +  XA*CY
      ZR =  ZA*CY +  XA*SY
      VXR=-VZA*SY + VXA*CY
      VZR= VZA*CY + VXA*SY
      XA = XR
      ZA = ZR
      VXA = VXR
      VZA = VZR
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
      VZP =  VZA  / VEL
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
    3 IF( DATA( 6,NO ) .EQ. 0. ) RETURN
c      IF( NP  .LE. 100) write (6, 102)
      XR =  XA*CZ +  YA*SZ
      YR = -XA*SZ +  YA*CZ
      VXR= VXA*CZ + VYA*SZ
      VYR=-VXA*SZ + VYA*CZ
      XA = XR
      YA = YR
      VXA = VXR
      VYA = VYR
      VXP = 1000. *DATAN2( VXA,VZA )
      VYP = 1000. *DASIN ( VYA/VEL )
c      IF( NP  .LE. 100) write (6, 103) TP, XA, YA, ZA, VZP, VXP, VYP
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT2 ( NUM, NO, NBR, TPAR )
      RETURN
      END


      SUBROUTINE SOLND  ( NO, NP, T, TP ,NUM )
C****
C****
C**** SOLENOID      RAY TRACING BY NUMERICAL INTEGRATION OF DIFFERENTIAL
C**** EQUATIONS OF MOTION.
C     T = TIME
C     TC(1) TO TC(6) =  ( X, Y, Z, VX, VY, VZ )
C     DTC(1) TO DTC(6) = ( VX, VY, VZ, VXDOT, VYDOT, VZDOT )
C**** BF (POSITIVE) : SOLENOID FIELD IN BEAM DIRECTION
C**** CBF - USED IN BSOL TO DISTINGUISH BETWEEN COORD. SYSTEMS
C****
C****
      IMPLICIT REAL*8(A-H,O-Z)
      REAL*8  LF           , K, L
      COMMON  /BLCK 0/  DATA , ITITLE
      COMMON  /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON  /BLCK 5/  XA, YA, ZA, VXA, VYA, VZA
      COMMON  /BLCK 7/ NCODE
      COMMON  /BLCK10/  BX, BY, BZ, K, TC, DTC
      COMMON  /BLCK30/  BF ,      AL, RAD
      COMMON  /BLCK31/  S, BT
      COMMON  /BLCK32/  IN
      DIMENSION DATA(  75,200 ), ITITLE(200)
      DIMENSION TC(6), DTC(6), DS(6), ES(6)
      EXTERNAL  BSOL
C**** DATA  C/ 3.D10/
C****
C****
      LF   = DATA(  1,NO )
      A    = DATA( 10,NO )
      B    = DATA( 11,NO )
      L    = DATA( 12,NO )
      D    = DATA( 13,NO )
      BF   = DATA( 14,NO )
      Z11  = DATA( 15,NO )
      Z22  = DATA( 16,NO )
      DTF1= LF/VEL
      AL  = L/2.
      RAD = D/2.
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S = 0.
C****
C****
      IF( NP  .GT. 100 ) GO TO 5
  201 FORMAT(  ' SOLENOID    ****  ', A4, '  ***********************'/)
c      write (6, 201) ITITLE(NO)
  101 FORMAT( 8H    T CM ,18X, 4HX CM , 7X, 2HBX, 8X, 4HY CM , 7X, 2HBY,
     1   8X, 4HZ CM, 7X, 2HBZ, 8X, 6HVELZ/C , 6X, 8HTHETA MR , 5X,
     2   6HPHI MR , 6X, 1HB             )
      CALL PRNT2 ( T,S,XA   ,YA   ,ZA   ,BX,BY,BZ,BT,VXA  ,VYA  ,VZA   )
c      write (6, 101)
c      write (6, 103)
  103 FORMAT(   '0COORDINATE TRANSFORMATION TO CENTERED AXIS SYSTEM ' )
  109 FORMAT(   '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM '       )
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES TO VFB COORD.
C****
    5 TC(1) =  XA
      TC(2) = YA
      TC(3) = ZA-A-AL
      TC(4) =  VXA
      TC(5) = VYA
      TC(6) =  VZA
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FIRST FRINGE FIELD
C****
      TDT = (-TC(3) -Z11 -AL ) /DABS( TC(6) )
C****
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 104)
  104 FORMAT( 22H0FRINGING FIELD REGION    )
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BSOL , 0    )
      NSTEP = 0
    6 CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      DO 7 I = 1, NP
      CALL FNMIRK( 6, T, DTF1,TC, DTC, DS, ES, BSOL , 1    )
      NSTEP = NSTEP + 1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      IF( (Z22+AL) .LE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 =-( TC(3) -(Z22+AL)  ) / DABS( TC(6) )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BSOL ,  0    )
      CALL FNMIRK( 6, T,XDTF1,TC, DTC, DS, ES,BSOL ,  1    )
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
c      IF( NP  .LE. 100) write (6, 105) NSTEP
  105 FORMAT( 10H   NSTEPS= , I5 )
C****
C**** TRANSFORM TO OUTPUT SYSTEM COORD.
C****
      TC(3) = TC(3) - B - AL
c      IF( NP  .LE. 100) write (6, 109)
      CALL PRNT2 ( T,S,TC(1),TC(2),TC(3),BX,BY,BZ,BT,TC(4),TC(5),TC(6) )
C****
C**** TRANSLATE PARTICLE TO OUT SYSTEM COORD.
C****
      TDT = -TC(3) /DABS( TC(6) )
      TC(1) = TC(1) + TDT * TC(4)
      TC(2) = TC(2) + TDT * TC(5)
      TC(3) = TC(3) + TDT * TC(6)
      T = T + TDT
      TP = T * VEL
      BX = 0.
      BY = 0.
      BZ = 0.
      BT = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      VZF    = TC(6) / VEL
c      IF(NP.LE.100) write (6,115) TP,TC(1),TC(2),TC(3),VZF,VXF,VYF
  115 FORMAT( F10.4, 10X, F10.3, 11X, F10.3, 11X, F10.3, 11X,
     1   F13.5, F13.2, F11.2                   )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** CALCULATE INTERCEPTS IN SYSTEM D
C****
      Z0X = -TC(1)/ ( TC(4) / TC(6)    + 1.E-10 )
      Z0Y = -TC(2)/ ( TC(5) / TC(6)    + 1.E-10 )
c      IF(NP.LE.100) write (6,111) VXF, VYF, Z0X, Z0Y
  111 FORMAT( / ' INTERSECTIONS WITH VER. AND HOR. PLANES '          ,
     X       /15X, 5H  XP=,F10.4, 10H MR    YP=, F10.4, 3H MR   /
     1        15X, 5H Z0X=,F10.2, 10H CM   Z0Y=,F10.2, 3H CM   /     )
      RETURN
   99 CALL PRNT4(NO, IN )
      RETURN
      END


      SUBROUTINE VELS ( NO,NP,T,TP ,NUM )
C****
C****
C     VELOCITY SELECTOR......ADDED JAN. 1976 BY W. R. BERNECKY
C****
C****
      IMPLICIT  REAL*8 (A-H,O-Z)
      REAL*8  K,LF1,LU1,LF2,L
      REAL*8  NDX
      EXTERNAL BEVC
      COMMON /BLCK 0/  DATA, ITITLE
      COMMON /BLCK 4/  ENERGY, VEL, PMASS, Q0
      COMMON /BLCK 5/  XA,YA,ZA,VXA,VYA,VZA
      COMMON /BLCK10/  BX,BY,BZ,K,TC,DTC
      COMMON /BLCK11/  EX, EY, EZ, QMC, IVEC
      COMMON /BLCK71/  CB0,CB1,CB2,CB3,CB4,CB5
      COMMON /BLCK72/  CE0,CE1,CE2,CE3,CE4,CE5
      COMMON /BLCK73/  IN,NFLAG
      COMMON /BLCK74/  BF,EF,S,DG
      COMMON /BLCK75/  BC2,BC4,EC2,EC4
      COMMON /BLCK76/  DB,DE,WB,WE
      COMMON /BLCK77/  RB,NDX
C****
      DIMENSION  DATA(75,200) , ITITLE(200)
      DIMENSION  TC(6),DTC(6),DS(6),ES(6)
C**** DATA  C/3.D10/
C****
      LF1=DATA( 1,NO)
      LU1=DATA( 2,NO)
      LF2=DATA( 3,NO)
      DG =DATA( 4,NO)
      A  =DATA( 7,NO)
      B  =DATA( 8,NO)
      L  =DATA( 9,NO)
      BF =DATA(10,NO)
      EF =DATA(11,NO)
      RB =DATA(12,NO)
      NDX=DATA(13,NO)
      DB =DATA(16,NO)
      DE =DATA(17,NO)
      WB =DATA(18,NO)
      WE =DATA(19,NO)
      Z11=DATA(20,NO)
      Z12=DATA(21,NO)
      Z21=DATA(22,NO)
      Z22=DATA(23,NO)
      BC2=DATA(24,NO)
      BC4=DATA(25,NO)
      EC2=DATA(26,NO)
      EC4=DATA(27,NO)
      NFLAG = 0
      IF( NDX .NE. 0. ) NFLAG=1
      IF( RB  .EQ. 0. ) RB=1.D30
      EX = 0.
      EY = 0.
      EZ = 0.
      S  = 0.
      BX = 0.
      BY = 0.
      BZ = 0.
      IF ( NP .GT. 100 ) GO TO 5
c      write (6, 100) ITITLE(NO)
  100 FORMAT ('0VELOCITY SELECTOR****  ',A4,'  ******************'/ )
c      write (6, 101)
  101 FORMAT (8H    T CM,6X,4HX CM,5X,2HBX,8X,2HEX,8X,4HY CM,5X,2HBY,8X,
     1       2HEY,7X,4HZ CM,6X,2HBZ,8X,2HEZ,6X,8HTHETA MR,5X,6HPHI MR,
     2   2X, 'VEL/E9'   )
      TDIST = T*VEL
      CALL PRNT3( TDIST,XA,YA,ZA,BX,BY,BZ,EX,EY,EZ,VXA,VYA,VZA )
c      write (6, 103)
  103 FORMAT ( '0COORDINATE TRANSFORMATION TO B AXIS SYSTEM' )
  109 FORMAT ( '0COORDINATE TRANSFORMATION TO D AXIS SYSTEM' )
C****
C**** TRANSFORM FROM INITIAL ENTRANCE COORDINATES
C****
    5 TC(1) = -XA
      TC(2) =  YA
      TC(3) = A-ZA
      TC(4) = -VXA
      TC(5) =  VYA
      TC(6) = -VZA
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE PARTICLE TO START OF FRINGE FIELD
C****
      TDT = ( TC(3)-Z11 )/DABS( TC(6) )
      TC(1) = TC(1)+TDT*TC(4)
      TC(2) = TC(2)+TDT*TC(5)
      TC(3) = TC(3)+TDT*TC(6)
      T = T+TDT
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** IN DESIGNATES MAGNET REGIONS FOR BFUN
C****
      IN = 1
      CB0=DATA(28,NO)
      CB1=DATA(29,NO)
      CB2=DATA(30,NO)
      CB3=DATA(31,NO)
      CB4=DATA(32,NO)
      CB5=DATA(33,NO)
      CE0=DATA(34,NO)
      CE1=DATA(35,NO)
      CE2=DATA(36,NO)
      CE3=DATA(37,NO)
      CE4=DATA(38,NO)
      CE5=DATA(39,NO)
      DTF1 = LF1/VEL
c      IF ( NP .LE. 100 ) write (6, 104)
  104 FORMAT ( 22H0FRINGING FIELD REGION)
      CALL FNMIRK (6,T,DTF1,TC,DTC,DS,ES,BEVC,0 )
      NSTEP = 0
      TDIST = T*VEL
    6 CONTINUE
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
      DO 7 I=1,NP
      CALL FNMIRK (6,T,DTF1,TC,DTC,DS,ES,BEVC,1 )
      NSTEP = NSTEP+1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 2
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      TDIST = TDIST + DTF1*VEL
      IF ( Z12 .GE. TC(3) ) GO TO 8
    7 CONTINUE
      GO TO 6
    8 CONTINUE
      XDTF1 = -( Z12-TC(3) )*DABS( TC(6) )/VEL**2
      CALL FNMIRK (6,T,XDTF1,TC,DTC,DS,ES,BEVC,0 )
      CALL FNMIRK (6,T,XDTF1,TC,DTC,DS,ES,BEVC,1 )
      TDIST = TDIST + XDTF1*VEL
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
c      IF ( NP .LE. 100 ) write (6, 105) NSTEP
  105 FORMAT ( 10H   NSTEPS= ,I5 )
C****
C****    TRANSLATE TO 2ND VFB COORDINATE SYSTEM
C****
      TC(1) = -TC(1)
      TC(3) = -(TC(3)+L)
      TC(4) = -TC(4)
      TC(6) = -TC(6)
C****
C**** UNIFORM FIELD REGION
C****
      IN = 2
      DTU = LU1/VEL
c      IF ( NP .LE. 100 ) write (6, 106)
  106 FORMAT ( '0UNIFORM FIELD REGION IN C AXIS SYSTEM' )
      CALL FNMIRK (6,T,DTU,TC,DTC,DS,ES,BEVC,0 )
      NSTEP = 0
    9 CONTINUE
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
      DO 10 I = 1,NP
      CALL FNMIRK (6,T,DTU,TC,DTC,DS,ES,BEVC,1 )
      NSTEP = NSTEP+1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      TDIST = TDIST + DTU*VEL
      IF ( TC(3) .GE. Z21 ) GO TO 11
   10 CONTINUE
      GO TO 9
   11 CONTINUE
      XDTU = (Z21-TC(3) )*DABS( TC(6) )/VEL**2
      CALL FNMIRK (6,T,XDTU,TC,DTC,DS,ES,BEVC,0)
      CALL FNMIRK (6,T,XDTU,TC,DTC,DS,ES,BEVC,1 )
      TDIST = TDIST + XDTU*VEL
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
c      IF ( NP .LE. 100 ) write (6, 105) NSTEP
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** SET UP FOR SECOND FRINGE FIELD INTEGRATION
C****
      CB0=DATA(40,NO)
      CB1=DATA(41,NO)
      CB2=DATA(42,NO)
      CB3=DATA(43,NO)
      CB4=DATA(44,NO)
      CB5=DATA(45,NO)
      CE0=DATA(46,NO)
      CE1=DATA(47,NO)
      CE2=DATA(48,NO)
      CE3=DATA(49,NO)
      CE4=DATA(50,NO)
      CE5=DATA(51,NO)
      IN = 3
      DTF2 = LF2/VEL
c      IF ( NP .LE. 100 ) write (6, 104)
      CALL FNMIRK (6,T,DTF2,TC,DTC,DS,ES,BEVC,0 )
      NSTEP=0
   12 CONTINUE
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
      DO 13  I=1,NP
      CALL FNMIRK (6,T,DTF2,TC,DTC,DS,ES,BEVC,1 )
      NSTEP = NSTEP+1
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF (NSTEP  .GT.  200)  GO TO 99
      TDIST = TDIST + DTF2*VEL
      IF ( TC(3) .GE. Z22 ) GO TO 14
   13 CONTINUE
      GO TO 12
   14 CONTINUE
      XDTF2 = ( Z22-TC(3) )*TC(6)/VEL**2
      CALL FNMIRK (6,T,XDTF2,TC,DTC,DS,ES,BEVC,0 )
      CALL FNMIRK (6,T,XDTF2,TC,DTC,DS,ES,BEVC,1 )
      TDIST = TDIST + XDTF2*VEL
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
c      IF (NP .LE. 100) write (6, 105) NSTEP
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 3
      CALL PLT1 ( NUM, NO, NBR, TPAR )
C****
C**** TRANSLATE TO OUTPUT COORDINATES
C****
      TC(3) = TC(3)-B
c      IF ( NP .LE. 100 ) write (6, 109)
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
      T = TDIST/VEL
      TDT =-TC(3)/DABS( TC(6) )
      TC(1) = TC(1)+TDT*TC(4)
      TC(2) = TC(2)+TDT*TC(5)
      TC(3) = TC(3)+TDT*TC(6)
      T = T+TDT
      BX = 0.
      BY = 0.
      BZ = 0.
      EX = 0.
      EY = 0.
      EZ = 0.
      S  = 0.
      VXF    = 1000. *DATAN2( TC(4), TC(6)  )
      VYF    = 1000. *DASIN ( TC(5)/ VEL    )
      TDIST = T*VEL
      NUM = NUM+1
      TPAR = T*VEL
      NBR = 4
      CALL PLT1 ( NUM, NO, NBR, TPAR )
      IF ( NP .GT. 100 ) GO TO 15
      CALL PRNT3 (TDIST,TC(1),TC(2),TC(3),BX,BY,BZ,
     1            EX,EY,EZ,TC(4),TC(5),TC(6)  )
   15 CONTINUE
      ZDX = -TC(1)/( TC(4)/TC(6)+1.E-10 )
      ZDY = -TC(2)/( TC(5)/TC(6)+1.E-10 )
c      IF (NP .LE. 100 ) write (6, 111) VXF,VYF,ZDX,ZDY
  111 FORMAT (/'0INTERSECTIONS WITH VER. AND HOR. PLANES '
     X       /15X,5H  XP=,F10.4,10H MR    YP=,F10.4,3H MR / ,
     1    15X,5H Z0X=,F10.2,10H CM   Z0Y=,F10.2,3H CM   /   )
      RETURN
   99 CALL PRNT4(NO, IN)
      RETURN
      END


      SUBROUTINE  RAYS(NR)
C****
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON  /BLCK 1/  XI, YI, ZI, VXI, VYI, VZI, DELP
C
C** xkine=K=(1/p)dp/dtheta
C** added by DY for spectrometer calculations 8/25/93
C
      COMMON  /BLCK15/  TMIN, PMIN, XMAX, TMAX, YMAX, PMAX, DMAX, xkine
C
      DIMENSION XI(1000),YI(1000),ZI(1000),VXI(1000),VYI(1000),
     1      VZI(1000), DELP(1000)
  100 FORMAT (///10X, 'JNR = ', I10 ///)
C****
C****
      DO 1 I=1,1000
      XI(I)=0.
      YI(I)=0.
      ZI(I)=0.
      VXI(I)=0.
      VYI(I)=0.
      VZI(I)=0.
      DELP(I)=0.
    1 CONTINUE
      IF (TMIN.EQ.0.) TMIN=1.0
      IF (PMIN.EQ.0.) PMIN=1.0
      TMAX2 = TMAX/2.0
      TMAX3 = TMAX/3.0
      PMAX2 = PMAX/2.0
      PMAX3 = 2.*PMAX/3.0
      IF (NR.EQ.2) GO TO 2
      IF (NR.EQ.6) GO TO 2
      IF (NR.EQ.14) GO TO 2
      IF (NR.EQ.46) GO TO 3
c      write (6,100) NR
      CALL EXIT
    2 VXI(2)=TMIN
      VYI(2)=PMIN
      IF (NR.EQ.2) GO TO 5
      VXI(3)=TMAX2
      VXI(4)=-TMAX2
      VXI(5)=TMAX
      VXI(6)=-TMAX
      IF (NR.EQ.6) GO TO 5
      VYI(7)=PMAX2
      VXI(8)=TMAX2
      VYI(8)=PMAX2
      VXI(9)=-TMAX2
      VYI(9)=PMAX2
      VXI(10)=TMAX
      VYI(10)=PMAX2
      VXI(11)=-TMAX
      VYI(11)=PMAX2
      VYI(12)=PMAX
      VXI(13)=TMAX2
      VYI(13)=PMAX
      VXI(14)=-TMAX2
      VYI(14)=PMAX
C****
C****
C****
    5 DO 4 I=1,NR
      XI(I) = XMAX
      YI(I) = YMAX
    4 DELP(I) = DMAX
C   
C   change DELP for RAY 2 for kinematic focusing DY 8/93
C
      if (xkine.ne.0.0) DELP(2)=-xkine*vxi(2)*.20
C
      RETURN
C****
C****
C****
    3 VXI(2)=TMIN
      VYI(2)=PMIN
C   
C   change DELP for RAY 2 for kinematic focusing DY 8/93
C
      if (xkine.ne.0.0) DELP(2)=-xkine*vxi(2)*.2
C
      XI(3)=XMAX
      XI(4)=-XMAX
      VXI(5)=TMAX3
      VXI(6)=-TMAX3
      YI(7)=YMAX
      YI(8)=-YMAX
      VYI(9)=PMAX3
      VYI(10)=-PMAX3
      DELP(11)=DMAX
      DELP(12)=-DMAX
      XI(13)=XMAX
      VXI(13)=TMAX3
      XI(14)=-XMAX
      VXI(14)=-TMAX3
      XI(15)=XMAX
      DELP(15)=DMAX
      XI(16)=-XMAX
      DELP(16)=-DMAX
      VXI(17)=TMAX3
      DELP(17)=DMAX
      VXI(18)=-TMAX3
      DELP(18)=-DMAX
      YI(19)=YMAX
      VYI(19)=PMAX3
      YI(20)=-YMAX
      VYI(20)=PMAX3
      XI(21)=XMAX
      YI(21)=YMAX
      XI(22)=-XMAX
      YI(22)=YMAX
      XI(23)=XMAX
      VYI(23)=PMAX3
      XI(24)=-XMAX
      VYI(24)=PMAX3
      VXI(25)=TMAX3
      YI(25)=YMAX
      YI(26)=YMAX
      VXI(27)=TMAX3
      VYI(27)=PMAX3
      VXI(28)=-TMAX3
      VYI(28)=PMAX3
      YI(29)=YMAX
      DELP(29)=DMAX
      YI(30)=YMAX
      DELP(30)=-DMAX
      VYI(31)=PMAX3
      DELP(31)=DMAX
      VYI(32)=PMAX3
      DELP(32)=-DMAX
      VXI(33)=TMAX
      VXI(34)=-TMAX
      XI(35)=XMAX
      VXI(35)=TMAX
      XI(36)=-XMAX
      VXI(36)=TMAX
      XI(37)=XMAX
      VXI(37)=-TMAX
      XI(38)=-XMAX
      VXI(38)=-TMAX
      VXI(39)=TMAX
      DELP(39)=DMAX
      VXI(40)=TMAX
      DELP(40)=-DMAX
      VXI(41)=-TMAX
      DELP(41)=DMAX
      VXI(42)=-TMAX
      DELP(42)=-DMAX
      VYI(43)=PMAX
      VXI(44)=TMAX
      VYI(44)=PMAX
      DELP(45)=3.*DMAX
      DELP(46)=-3.*DMAX
      RETURN
      END
C******************************
C                             *
C  FILE:  DATIME.F            *
C                             *
C******************************
C
C  THIS FILE CONTAINS SUBROUTINES THAT INTERFACE THE GETDAT AND GETTIM
C   CALLS OF MICROSOFT FORTRAN, ETC., TO THE APPROPRIATE CALLS FOR
C   THE MICROWAY NDP-FORTRAN 386 COMPILER.
C
C  THE CALLING SEQUENCES ARE OBVIOUS.  NOTE THAT THE SUBROUTINE
C   ARGUMENTS ARE INT*2 TO BE COMPATIBLE WITH OTHER PC COMPILERS.
C   THIS MAY BE A PROBLEM FOR PROGRAMS PORTED FROM THE VAX.
C
 
c******
c        SUBROUTINE GETDAT(IYEAR,IMON,IDAY)
c        INTEGER*2 IYEAR,IMON,IDAY
C
c        INTEGER*4 IMO,IDA,IYR,IDUMMY
c        CALL DOSDAT(IMO,IDA,IYR,IDUMMY)
c        IYEAR=IYR
c        IMON=IMO
c        IDAY=IDA
c        RETURN
c        END
C
C

c        SUBROUTINE GETTIM(IHOUR,IMIN,ISEC,IHUND)
c        INTEGER*2 IHOUR,IMIN,ISEC,IHUND
c
c        INTEGER*4 IHR,IMN,ISC,IHU
c        CALL DOSTIM(IHR,IMN,ISC,IHU)
c        IHOUR=IHR
c        IMIN=IMN
c        ISEC=ISC
c        IHUND=IHU
c        RETURN
c        END
c******


       SUBROUTINE KINE(AM,THETA,ENGY,RAT,PCTR,KB)
CC     THIS ROUTINE PERFORMS CALCULATIONS FOR FOUR BODY RELATIVISTIC
CC     SCATTERING, IF THE INPUT DATA HAS NO SOLUTION, THE VALUE OF KB
CC     ON RETURN IS ONE (1). OTHERWISE, KB = 2.
C
CC     INPUT.....
CC     AM(1) TO AM(3) ARE THE MASSES (IN C12 SCALE AMU) OF THE INCIDENT 
CC     TARGET, AND SCATTERED PARTICLES, RESPECTIVELY.
CC     THETA(1) IS THE LAB SCATTERING ANGLE (IN DEGREES).
CC     ENGY(1) IS THE Q-VALUE (IN MEV) OF THE REACTION
CC     ENGY(2) IS THE BEAM ENERGY (IN MEV).
C
CC     OUTPUT.....
CC     AM(4) IS THE MASS (IN C12 SCALE AMU) OF THE RESIDUAL NUCLEUS.
CC     THETA(2) IS THE RECOIL ANGLE (IN DEGREES) OF MASS FOUR (4).
CC     THETA (3) IS THE CENTER-OF-MOMENTUM ANGLE (IN DEGREES) OF SCATTER
CC     ENGY(3) IS THE KINETIC ENERGY (IN MEV) OF THE SCATTERED PARTICLE.
CC     RAT(1) IS THE KINEMATIC BROADENING (IN KEV/DEGREE) OF THE PRODUCT
CC     RAT(2) IS THE CROSS-SECTION RATIO -- SIGC/SIGL.
C
CC     PCTR IS THE MOMENTUM TRANSFER
C
CC     PROGRAM CALCULATES SECOND SOLUTION IF AM(4) IS SET NEGATIVE
CC     FUNCTION =1 IF NO SOLUTION AND ENGY(3) .LT. 0.0
C
	IMPLICIT REAL*8 (A-H,P-Z)
C	DIMENSION EM(3)
C	REAL*4 AM(4),THETA(3),ENGY(3),RAT(2),PCTR
        DIMENSION AM(4), THETA(3), ENGY(3), RAT(2), EM(3)
C
C	WRITE (*,31) (AM(I),I=1,4),(THETA(I),I=1,3),(ENGY(I),I=1,3)
C31	FORMAT (10F8.2)
C	
	KDEL = 1
	ISOLU=0
	IF(AM(4).LT.0.0) ISOLU=1
	KB = 1
        ENGY(3)=-0.001
C	write (*,*) ' in kine am(4) ',am(1),am(2),am(3),am(4)
 	IF((AM(4).LT.0.0).AND.(AM( 1).LE.AM(2))) GO TO 24
	IF((AM(1).GT.AM(2)).AND.(THETA(1).GT.90.0)) GO TO 24
C  set theta to 0.01 for 0 deg. elastic scattering
	IF (THETA(1) .EQ. 0.0 ) THETA(1) = 0.01
	Q = ENGY(1)
	TA = ENGY(2)
	THL= .01745329*THETA(1)
	DO 10 I=1,3
 10	EM(I)= 931.502*AM(I)
  	EMI= EM(1)+EM(2)
 	AM(4)= (EMI-EM(3)-Q)/931.502
C	write (*,*) ' em',em(1),em(2),em(3),emi,am(1),am(2),am(3)
 	COSL= COS (THL)
  	SINL= SIN (THL)
  	ETOT=EMI+TA
  	PA= SQRT (TA*(2.*EM(1)+TA))
  	BETA= PA/ETOT
 	GAMMA=ETOT/SQRT(EMI*EMI+2.*TA*EM(2))
C	write (*,*) ' in kine 1.5',pa,ta,eps,beta,cosl,etot,emi
  	EPS= BETA*COSL
	EPSQ= EPS*EPS
 	OMES= 1.-EPSQ
 	EPSMC= EPS*EM(3)
 	B= (Q*(EMI-EM(3)-(Q/2.))+TA*(EM(2)-EM(3)))/ETOT
 	BP= B+EM(3)*EPSQ
 	DEL= B*B+EM(3)*(B+BP)
C	write (*,*) ' in kine2', del
 	IF (DEL) 24, 12, 13
12	KDEL = 2
  	RAT(1)= 0.
  	GO TO 14
13	DEL= SQRT (DEL) 
14	TC=(BP+(-1.)**ISOLU*EPS*DEL)/OMES
C	write (*,*) ' in kine3',eps,del,omes,bp
 	 IF (TC-.001) 24,16,16
16	KB=2
	ENGY(3)=TC
  	GO TO (18,20), KDEL
18	ZZ=2.*(-1.)**ISOLU
 	RAT(1)=ABS((ZZ*EPS*TC+EPSMC*(2.+EPSMC/DEL)+DEL)*BETA*SINL*
     1		17.45329/OMES)
20	PC=SQRT(TC*(2.*EM(3)+TC))
 	PD=SQRT(ABS(PA*PA+PC*(PC-2.*PA*COSL)))
C
	TPC = PC*SINL
C	WRITE (*,30) PC,SINL,PD,PA,TC,TPC
C30	FORMAT (6F15.8)
	PHI=ASIN(PC*(SINL/PD))
 	IF (PA-PC*COSL) 25,27,27
25	PHI= 3.1415926-PHI
27	CONTINUE
	THETA(2)=PHI*57.295779
 	PCPX=GAMMA*(PC*COSL-BETA*(EM(3)+TC))
 	PCP= SQRT (PCPX*PCPX+(PC*SINL)**2)
 	THC= ASIN (PC*SINL/PCP)
 	COSC= COS (THC)
	IF (PCPX) 26,28,28
26 	THC= 3.1415926-THC
 	COSC= -COSC
28	THETA(3)= THC*57.295779
 	RAT(2)= GAMMA*(1.+COSC*BETA*SQRT(1.+(EM(3)/PCP)**2))*(PCP/PC)**3
 	RAT(2)=ABS(RAT(2))
 	COSTH=COS(THC)
 	ECM=SQRT(EMI*EMI+2.*TA*EM(2))
 	ECM1=(EM(1)*EMI+TA*EM(2))/ECM
	TCM1=ECM1-EM(1)
	PCM1=SQRT(TCM1*(TCM1+2.*EM(1)))
  	PCTR=0.005067*SQRT(PCM1*PCM1+PCP*(PCP-2.*PCM1*COSTH))
24 	AM(4)=ABS(AM(4))
	RETURN
 	END
