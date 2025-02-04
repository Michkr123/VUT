// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// (tabs)/_layout.tsx - Spodní prvek navigace
// Autoři: Petr Hron (xhronpe00)
//         Marek Antoňů (xanton07)

import FontAwesome from '@expo/vector-icons/FontAwesome';
import { Tabs } from 'expo-router';
import React from 'react';

export default function TabLayout() {
  return (
    // Jednotlivé záložky dostupné v zápatí aplikace
    <Tabs screenOptions={{ tabBarActiveTintColor: 'blue'}}>
      <Tabs.Screen
        name="index"
        options={{
          title: 'Akce pro lidi',
          tabBarIcon: ({ color }) => <FontAwesome size={28} name="home" color={color} />,
        }}
      />
      <Tabs.Screen
        name="calendar"
        options={{
          title: 'Kalendář',
          tabBarIcon: ({ color }) => <FontAwesome size={28} name="calendar" color={color} />,
        }}
      />
      <Tabs.Screen
        name="profile"
        options={{
          title: 'Profil',
          tabBarIcon: ({ color }) => <FontAwesome size={28} name="user" color={color} />,
        }}
      />
    </Tabs>
  );
}
