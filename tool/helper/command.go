package helper

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"os/exec"
)

func RunCommand(env map[string]string, command string, parameters ...string) (string, error) {
	cmd := exec.Command(command, parameters...)
	environment := os.Environ()
	for k, v := range env {
		environment = append(environment, fmt.Sprintf("%s=%s", k, v))
	}
	cmd.Env = environment
	var stdOut bytes.Buffer
	var stdErr bytes.Buffer
	cmd.Stdout = &stdOut
	cmd.Stderr = &stdErr
	err := cmd.Run()
	if err != nil {
		errStr := stdErr.String()
		log.Print(errStr)
		return errStr, err
	}
	return stdOut.String(), nil
}
