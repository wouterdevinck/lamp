package helper

import (
	"bytes"
	"os/exec"
)

func RunCommand(command string, parameters ...string) (string, error) {
	cmd := exec.Command(command, parameters...)
	var stdOut bytes.Buffer
	var stdErr bytes.Buffer
	cmd.Stdout = &stdOut
	cmd.Stderr = &stdErr
	err := cmd.Run()
	if err != nil {
		return stdErr.String(), err
	}
	return stdOut.String(), nil
}
