# Changelog

## V1.2

April 5, 2025

Bugfixes and Updates

* Fixed overflow bug on solution test counter.

* Updated the simplify function to allow new algorithms to be added more easily.

* Added Hidden Single algorithum. Siginificantly improves puzzle with many blanks.

* Added Naked Pair detection and revized simplify function on only reduce complexity and dose not repopulate possible values, only prunes.

* Optimized memory usage to increas speed of simplify and solver

Known Issues

* The simplify phase now takes longer to run. This this is helpful in puzzles with many blanks, but puzzles with fewer blanks may solve faster with bruit force approach.

* Some puzzles are still resistance to burit force solving
