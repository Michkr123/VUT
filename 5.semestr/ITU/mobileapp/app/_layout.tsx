// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// app/_layout.tsx - Popis jednotlivých stránek
// Autoři: Petr Hron (xhronpe00)
//         Marek Antoňů (xanton07)

import { Stack } from 'expo-router/stack';
import { StatusBar } from 'expo-status-bar';
import React from 'react';
import { UserProvider } from '../context/UserContext';

export default function Layout() {
  return (
    // Poskytnutí funkce useUser všem potomkům (v tomto případě celé aplikaci)
    <UserProvider>

      {/* Všechny možné stránky dostupné v naší aplikaci */}
      <Stack>
        <Stack.Screen name="(tabs)" options={{ headerShown: false }} />
        <Stack.Screen 
          name="(profile)/edit" 
          options={{ 
            title: 'Upravit profil',
            headerStyle: {
              backgroundColor: '#f5f5f5',
            },
            headerTintColor: '#333',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
          }} 
        />
        <Stack.Screen 
          name="(events)/[id]" 
          options={{ 
            title: 'Přehled akce',
            headerStyle: {
              backgroundColor: '#f5f5f5',
            },
            headerTintColor: '#333',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
          }} 
        />
        <Stack.Screen 
          name="(events)/review" 
          options={{ 
            title: 'Recenze',
            headerStyle: {
              backgroundColor: '#f5f5f5',
            },
            headerTintColor: '#333',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
          }} 
        />
        <Stack.Screen 
          name="(events)/create" 
          options={{ 
            title: 'Tvorba nové akce',
            headerStyle: {
              backgroundColor: '#f5f5f5',
            },
            headerTintColor: '#333',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
          }} 
        />
        <Stack.Screen 
          name="(events)/edit/[id]" 
          options={{ 
            title: 'Upravit událost',
            headerStyle: {
              backgroundColor: '#f5f5f5',
            },
            headerTintColor: '#333',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
          }} 
        />
      </Stack>
      <StatusBar style="dark" /> {/* Aplikace využívá světlých barev, chceme tmavou hlavičku */}
    </UserProvider>
  );
}
