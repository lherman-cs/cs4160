package main

import (
	"os"
	"strconv"
	"time"
)

func getEnv(key, fallback string) string {
	if value, ok := os.LookupEnv(key); ok {
		return value
	}
	return fallback
}

func getEnvInt(key string, fallback int) int {
	valueStr := getEnv(key, strconv.Itoa(fallback))
	if value, err := strconv.Atoi(valueStr); err != nil {
		return value
	}
	return fallback
}

func delayShowDice() time.Duration {
	delay := getEnvInt("DELAY_SHOW_DICE", 0)
	return time.Duration(delay) * time.Millisecond
}

func delayAfterCall() time.Duration {
	delay := getEnvInt("DELAY_AFTER_CALL", 0)
	return time.Duration(delay) * time.Millisecond
}
