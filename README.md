Introduction
============

What is spamster?
-----------------

Spamster is a collection of software that helps in classification of
ham/spam emails. The name of the project is a word-play on "hamster".

This directory contains the software of Naive Bayes Spam Detector and
Support Vector Machine Spam Detector.

Naive Bayes Spam Detector
-------------------------

Naive Bayes Spam Detector is an experimental e-mail spam filter that
can be configured to use various forms of Naive Bayes classifier. The
various forms of Naive Bayes classifiers that can be used are
appropriately documented in the doc/nb directory.

The filter is implemented in C++. Its source code can be compiled
with GCC. The filter is currently configured to be used from a Unix
bash shell. It should be possible to use it on other systems with
minor modifications.

The software of the filter is released with the GNU General Public
License; please consult the file COPYING.txt for more information.

-------------------------------------------------------------------
    THIS SOFTWARE IS A RESEARCH PROTOTYPE AND IS PROVIDED WITH
      ABSOLUTELY NO GUARANTEE AND ABSOLUTELY NO SUPPORT!
-------------------------------------------------------------------

Support Vector Machine based Spam Detector
------------------------------------------

Support Vector Machine (SVM) based Spam Detector is an elementary
e-mail spam filter that uses a linear kernel to classify between
spam and ham messages. Documentation of this software can be found
in doc/svm directory.

This filter is implemented in R. Its source code can be executed
with R, with certain modules that can be found in CRAN. Refer to
doc/svm for more information.

The software of the filter is released with the GNU General Public
License; please consult the file COPYING.txt for more information.

-------------------------------------------------------------------
    THIS SOFTWARE IS A RESEARCH PROTOTYPE AND IS PROVIDED WITH
      ABSOLUTELY NO GUARANTEE AND ABSOLUTELY NO SUPPORT!
-------------------------------------------------------------------

Inspirations for Naive Bayes Spam Detector:
===========================================

The initial versions of Naive Bayes Spam Detector software was
developed by Aris Kosmopoulos, and others during the following MSc
thesis (in Greek):

Aris Kosmopoulos, "E-mail spam filtering with various forms of the
Naive Bayes classifier and filter sharing among users", MSc thesis,
Department of Informatics, Athens University of Economics and Business,
2007. Available from:
<http://www.aueb.gr/users/ion/docs/kosmopoulos_msc_thesis.pdf> and
<http://pages.cs.aueb.gr/nlp/theses/kosmopoulos_msc_thesis.pdf>.

The filter is also described in the following paper (in English):

Aris Kosmopoulos, Georgios Paliouras and Ion Androutsopoulos,
"Adaptive spam filtering using only Naive Bayes text classifiers".
In the Spam Filtering Challenge Competition of the 5th Conference
on Email and Anti-Spam (CEAS 2008), Mountain View, California, 2008.
Available from:
<http://ceas.cc/2008/papers/askomo.pdf>,
<http://www.aueb.gr/users/ion/docs/ceas2008_paper.pdf> and
<http://pages.cs.aueb.gr/nlp/pubs/ceas2008_paper.pdf>

The inspiration for attacking the Naive Bayes Spam Detector and the
SVM based Spam Detector was taken from:

Battista Biggio, Blaine Nelson, and Pavel Laskov, "Poisoning Attacks
against Support Vector Machines",
Ling Huang, Anthony D. Joseph, Blaine Nelson, Benjamin I. P.
Rubinstein, and J. D. Tygar, "Adversarial Machine Learning",
Battista Biggio, Giorgio Giacinto, and Fabio Roli, "Bagging
Classifiers for Fighting Poisoning Attacks in Adversarial
Classification Tasks"

Available from:
<http://arxiv.org/pdf/1206.6389.pdf>,
<http://blaine-nelson.com/research/pubs/Huang-Joseph-AISec-2011>,
<http://prag.diee.unica.it/pra/system/files/biggio-poisoning-mcs11.pdf>

Data sets
=========

The data sets used to train these algorithms can be found on
<https://spamassassin.apache.org/publiccorpus/>, though one could use
any of the several available corpora to train the above algorithms.

Some of the considered corpora can be found in the doc/corpora.md.

Source code
===========

The source code can be found in the following directory:

Naive Bayes Spam Detector: 	naive/src
SVM based Spam Detector:	svm/

Copyright (c) 2013 Anuj More.
Please send bug reports to Anuj More (anujmorex AT gmail DOT com) for
most of the parts of Naive Bayes Spam Detector, SVM Spam Detector,
and the documentation.

Aris Kosmopoulos (akosmo AT iit DOT demokritos DOT gr) for certain sections
of the Naive Bayes Spam Filter.

------ END OF FILE ------
