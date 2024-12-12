import { Stack } from 'expo-router/stack';
import { StatusBar } from 'expo-status-bar';
import React from 'react';

export default function Layout() {
  return (
    <>
    <Stack>
      <Stack.Screen name="(tabs)" options={{ headerShown: false }} />
    </Stack>
    <StatusBar style="dark" />
    </>
  );
}
