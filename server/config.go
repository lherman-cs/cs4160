package main

import (
	"os"
	"strconv"
	"time"

	log "github.com/sirupsen/logrus"
)

var (
	delayShowDice  = time.Duration(getEnvInt("DELAY_SHOW_DICE", 0)) * time.Millisecond
	delayAfterCall = time.Duration(getEnvInt("DELAY_AFTER_CALL", 0)) * time.Millisecond
	delayBotMin    = getEnvInt("DELAY_BOT_MIN", 1000)
	delayBotMax    = getEnvInt("DELAY_BOT_MAX", 2000)
)

func getEnv(key, fallback string) string {
	if value, ok := os.LookupEnv(key); ok {
		return value
	}
	return fallback
}

func getEnvInt(key string, fallback int) int {
	valueStr := getEnv(key, strconv.Itoa(fallback))
	if value, err := strconv.Atoi(valueStr); err == nil {
		return value
	}
	return fallback
}

func init() {
	entry := log.WithField("file", "config.go")
	entry.Info("DELAY_SHOW_DICE=", delayShowDice)
	entry.Info("DELAY_AFTER_CALL=", delayAfterCall)
	entry.Info("DELAY_BOT_MIN=", delayBotMin)
	entry.Info("DELAY_BOT_MAX=", delayBotMax)
}
