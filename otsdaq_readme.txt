###############################
So, you want to run the FTBF daq using otsdaq? Here are the three steps!

1. Log into ftbf-daq-08.fnal.gov as ftbf_user (once logged in do this: ksu ftbf_user).


2. Navigate to /home/nfs/ftbf_user/Desktop (e.g.: cd ~/Desktop).
   ... less otsdaq_readme.txt #to view this readme

3. At this point you probably want to do one of three things:
   3a. Start otsdaq
   3b. Use otsdaq (i.e. otsdaq is already running)
   3c. Start otsdaq as a particular experiment run

   ... the details for 3a, 3b, and 3c are below.

   - all data for your run will end up here: ll $OTSDAQ_DATA/


3a. Start otsdaq:

    ./setup_otsdaq.sh --run --firefox

       - The above line sets up otsdaq, runs it, and opens the web GUI in firefox.
       - When firefox opens, you might have to select a user; select any user you like, e.g. DEFAULT_USER.
       - Note!!! For fermilab web security to be happy, we access the web GUI through https, so the first time you must add an exception in your browser for the security certificate. 
       (In Firefox, click "Advanced" --> "Add Exception..." --> "Confirm Security Exception"; in Chrome, click "Advanced" --> "Proceed to ftbf-daq-08.fnal.gov (unsafe)")
       - The web GUI will be faster/smoother if you use firefox or chrome on your local computer; https://ftbf-daq-08.fnal.gov is accessible anywhere on-site at Fermilab.
       (You may need a CILogin certificate: follow instructions here https://cilogon.org/)

3b. Use otsdaq (i.e. otsdaq is already running): 

    ./setup_otsdaq.sh --firefox

       - The above line sets up otsdaq, does NOT runs it, and opens the web GUI in firefox (you could also just open firefox manually and navigate to the url).


3c.  Start otsdaq as a particular experiment run:

     ./setup_otsdaq.sh --run --experiment <name of your experimental run>

       - The above line sets up otsdaq, creates a new user data and database area, and runs otsdaq.
       - Note!!! your expreriment run name should be alpha-numeric with underscores and dashes only.
       	   e.g. 2018_OCTOBER_T992

