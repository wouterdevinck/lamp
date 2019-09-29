package provision

import (
	"bytes"
	"encoding/hex"
	"encoding/json"
	"errors"
	"factory/helper"
	"factory/model"
	"factory/protocol"
	"fmt"
	"time"
)

func configure(ctx *Context) error {

	// Check if config is locked
	locked, err := ctx.protocol.IsConfigLocked()
	if err != nil {
		return err
	}

	// Read the current device config
	actConf, err := getDeviceConfig(ctx.protocol)
	if err != nil {
		return err
	}

	// Read the desired config from file
	desConf, err := readConfigFromFile(ctx.configPath)
	if err != nil {
		return err
	}

	// Compare actual and desired configuration
	configured := !bytes.Equal(actConf[16:], desConf)

	if locked {
		if configured {
			// No action required
			fmt.Println("Device is already configured and configuration is locked")
		} else {
			// Abort: incorrect configuration cannot be fixed
			path := time.Now().Format("config-01022006-150405.json")
			err = writeConfigToFile(actConf, path)
			if err != nil {
				fmt.Println("Failed to write configuration to file")
			}
			return errors.New("configuration is locked but incorrect")
		}
	} else {
		if !configured {
			err = ctx.protocol.WriteConfig(hex.EncodeToString(desConf))
			if err != nil {
				return err
			}
			fmt.Println("Configuration written to device")
		} else {
			fmt.Println("Device is already configured correctly")
		}
		err = ctx.protocol.LockConfig()
		if err != nil {
			return err
		}
		fmt.Println("Device configuration locked")
	}
	return nil
}

func getDeviceConfig(protocol *protocol.Protocol) ([]byte, error) {
	conf, err := protocol.ReadConfig()
	if err != nil {
		return nil, err
	}
	return hex.DecodeString(conf)
}

func readConfigFromFile(path string) ([]byte, error) {
	confJSON, err := helper.ReadFile(path)
	if err != nil {
		return nil, err
	}
	var conf model.Config
	err = json.Unmarshal([]byte(confJSON), &conf)
	if err != nil {
		return nil, err
	}
	return model.WriteBinaryConfig(&conf, false)
}

func writeConfigToFile(conf []byte, path string) error {
	config, err := model.ParseBinaryConfig(conf, true)
	if err != nil {
		return err
	}
	jsonBytes, err := json.MarshalIndent(config, "", "  ")
	if err != nil {
		return err
	}
	return helper.WriteFile(path, string(jsonBytes))
}
