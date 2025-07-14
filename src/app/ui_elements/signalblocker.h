/* prevent unintended propagation of signals
 *
 * A class template for a blocker proxy object. It blocks the signals on construction, and then restores their previous
 * state on destruction. It also overloads the -> operator to return a pointer to the blocked object.
 *
 * https://stackoverflow.com/questions/3556687/prevent-firing-signals-in-qt
 *
 * Alternative 1 (not as elegant):
 *      Qt's QSignalBlocker class
 * Alternative 2 (also not as elegant):
 *      myObject->blockSignals(true);
 *      myObject->setValue(123);
*       myObject->blockSignals(false);
 * */

template<class T> class Blocker {
    T *blocked;
    bool previous;
public:
    Blocker(T* blocked) : blocked(blocked),  previous(blocked->blockSignals(true)) {}
    ~Blocker() { blocked->blockSignals(previous); }
    T *operator->() { return blocked; }
};

// A small template function to construct and return a Blocker
template<class T> inline Blocker<T> whileBlocking(T *blocked) {
    return Blocker<T>(blocked);
}
