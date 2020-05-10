pyfbow
==============

Ultra-fast Boost.Python interface for `fbow <https://github.com/rmsalinas/fbow>`_

This repository was created based on `pyORBSLAM2 <https://github.com/foxis/pyDBoW3>`

.. note::

  Tested on these platforms:

.. _install:

Get started
-----------

Unix
++++
Use ``build.sh`` to build ``build/pyfbow.so``, which you should then put on your ``PYTHONPATH``.


Using under a conda environment (to use pre-installed version of OpenCV)
++++
Build a conda environment, adding boost and cmake to it, something like:

.. code-block:: bash
   
   conda create -n test_env python=3.5 opencv=3.3.1 cmake boost matplotlib numpy
   
