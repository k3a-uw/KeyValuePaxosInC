#!/bin/bash
USERNAME=k3a


# THE FOLDER THAT CONTAINS THE SOURCE CODE _AND_ THE CSSGATE.SH SCRIPT  (NO TRAILING SLASH)
SRC_FOLDER=~/git/TCSS558/src

echo "Copying to CSSGATE, enter your cssgate password if prompted."
scp -o StrictHostKeyChecking=no -r ${SRC_FOLDER} ${USERNAME}@cssgate.insttech.washington.edu:~/win


echo "Executing remote script on cssgate, please enter your cssgate password if prompted"
ssh ${USERNAME}@cssgate.insttech.washington.edu '~/win/src/cssgate_deploy.sh'
