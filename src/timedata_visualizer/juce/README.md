The plan so far.
------

* The juce application only receives messages representing class descriptions
and sends back class proxies - through a pair of queues.

* Each class proxy also has its own pair of queues.

* A method call is a global function and data.

* You can detect whether your process is Juce or master by looking at _RECEIVE.

* If you're master, when you call a function, you delegate it down the queue.

* If you're in Juce, you call the function on yourself with the given data!

There's the C++ class, LightWindow.
The Cython class _LightWindow (called LightWindow in Python).
The description class, LightWindowDesc in C++, _LightWindowDesc in Cython.
