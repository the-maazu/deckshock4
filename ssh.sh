#!/bin/bash
# login into steamdeck
source .env
ssh -i $SSH_KEY $JUMP_HOST $USER@$HOST -o "LogLevel=DEBUG"