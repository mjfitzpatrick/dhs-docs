class ObservationSet {
   public:
      ObservationSet();
      ~ObservationSet();
   private:
      int expectedExposures;
      vector <Exposure*> recvdExposures;
      char *obsSet_id;
};

/* Perhaps we need a command to signal the beginning of a new set of       *
 * observations.  Could we rename dhsOpenConnect() to dhsOpenObsSet()?     *
 * Then a PAN/client will keep a connection to a Collector for the         *
 * duration of the observation set and let it go afterwards, obtaining a   *
 * new connection from the Supervisor for the next observation set.        */

class Exposure {
   public:
      Exposure();
      ~Exposure();
   private:
      /* track each half of pixel data, 4 pieces of detector metadata (all the same???), and one global metadata - store location in scp format machine:path/filename except it assumes files... */
      double exp_id;
      /* hard-code focal plane and how PANS will split? */
