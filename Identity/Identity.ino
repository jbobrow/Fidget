/*
   Identity

   This Sketch Let's each of the following arrangments self identify

   5 unique states of 3 Blinks

   1. Alone
   2. Paired
   3. Straight (end or center)
   4. Bent (end or center)
   5. Triangle

   Each of the Blinks can be in these states, and in the case of
   Straight or Bent, they can be an end or the center piece.

*/

enum States {
  ALONE,
  PAIRED,
  STRAIGHT,
  BENT,
  TRIANGLE,
  NUM_STATES
};

byte state;
bool neighbors[6];
bool isEndPiece;

bool endPiece[6] = {1, 0, 0, 0, 0, 0};
bool bent[6] = {1, 0, 1, 0, 0, 0};
bool straight[6] = {1, 0, 0, 1, 0, 0};
bool triangle[6] = {1, 1, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // check neighbors
  FOREACH_FACE(f) {
    neighbors[f] = !isValueReceivedOnFaceExpired(f);  // store if the neighbor is present
  }

  state = getState();

  switch (state) {
    case ALONE:
      aloneLoop();
      break;
    case PAIRED:
      pairedLoop();
      break;
    case STRAIGHT:
      straightLoop();
      break;
    case BENT:
      bentLoop();
      break;
    case TRIANGLE:
      triangleLoop();
      break;
  }
}

/*

*/
void aloneLoop() {
  setColor(RED);
}

/*

*/
void pairedLoop() {
  setColor(GREEN);
}

/*

*/
void straightLoop() {
  setColor(YELLOW);
}

/*

*/
void bentLoop() {
  setColor(ORANGE);
}

/*

*/
void triangleLoop() {
  setColor(WHITE);
}

/*
   evaluate arrangement and determine which state I am in
   if I am in state 3 or 4, also determine if I am the end or
   center
*/
byte getState() {
  byte _state;
  
  isEndPiece = false; // only set this true if I have one neighbor

  if (isAlone()) {
    _state = ALONE;
  }
  else if (isThisPatternPresent(endPiece, neighbors)) {
    isEndPiece = true;
    setValueSentOnAllFaces(PAIRED);

    // look to neighbor to determine the state
    byte neighborVal;
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        neighborVal = getLastValueReceivedOnFace(f);
      }
    }
    if (neighborVal == BENT) {
      _state = BENT;
    }
    else if (neighborVal == STRAIGHT) {
      _state = STRAIGHT;
    }
    else if (neighborVal == PAIRED) {
      _state = PAIRED;
    }
  }
  else if (isThisPatternPresent(bent, neighbors)) {
    setValueSentOnAllFaces(BENT);
    _state = BENT;
  }
  else if (isThisPatternPresent(straight, neighbors)) {
    setValueSentOnAllFaces(STRAIGHT);
    _state = STRAIGHT;
  }
  else if (isThisPatternPresent(triangle, neighbors)) {
    setValueSentOnAllFaces(TRIANGLE);
    _state = TRIANGLE;
  }

  return _state;
}

// check to see if pattern is in the array
// return true if the pattern is in fact in the array
// pattern is always 6 bools
// source is always 12 bools (2 x 6 bools)
bool isThisPatternPresent( bool pat[], bool source[]) {

  // first double the source to be cyclical
  bool source_double[12];

  for (byte i = 0; i < 12; i++) {
    source_double[i] = source[i % 6];
  }

  // then find the pattern
  byte pat_index = 0;

  for (byte i = 0; i < 12; i++) {
    if (source_double[i] == pat[pat_index]) {
      // increment index
      pat_index++;

      if ( pat_index == 6 ) {
        // found the entire pattern
        return true;
      }
    }
    else {
      // set index back to 0
      pat_index = 0;
    }
  }

  return false;
}

/*
   This function returns the number of neighbors I have
*/
byte getNumNeighbors() {

  byte numNeighbors = 0;

  FOREACH_FACE(f) { // look at each of my faces
    if (!isValueReceivedOnFaceExpired(f)) { // a neighbor is present
      numNeighbors++; // add one to my count
    }
  }

  return numNeighbors;
}
