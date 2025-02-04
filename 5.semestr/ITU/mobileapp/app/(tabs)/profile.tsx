// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// profile.tsx - Karta profilu uživatele
// Autoři: Petr Hron (xhronpe00)
//         Marek Antoňů (xanton07)

import { Profile } from '@/types/Profile';
import FontAwesome from '@expo/vector-icons/FontAwesome';
import { useFocusEffect } from '@react-navigation/native';
import { router } from 'expo-router';
import React, { useState } from 'react';
import { ActivityIndicator, StyleSheet, Text, TouchableOpacity, View } from 'react-native';
import { BACKEND_IP, BACKEND_PORT } from '../../backend_address';
import { useUser } from '../../context/UserContext';

export default function ProfileTab() {
  const { login, setLogin } = useUser();
  const [profile, setProfile] = useState<Profile | null>(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  // Načtení dat profilu ze serveru
  const fetchProfile = async () => {
    try {
      setIsLoading(true);
      setError(null);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      setProfile(data);
    } catch (error) {
      console.error("Error fetching profile:", error);
      setError("Nepodařilo se načíst profil.");
    } finally {
      setIsLoading(false);
    }
  };

  // Obnovení dat při návratu na obrazovku
  useFocusEffect(
    React.useCallback(() => {
      fetchProfile();
    }, [login])
  );

  // Přepínání role uživatele (admin/user)
  const handleRoleSwitch = (newRole: "user" | "admin") => {
    if (newRole !== login) {
      setLogin(newRole);
    }
  };

  // Zobrazení načítání
  if (isLoading) {
    return (
      <View style={styles.loadingOverlay}>
        <ActivityIndicator size="large" color="#2196f3" />
      </View>
    );
  }

  // Zobrazení chyby
  if (error || !profile) {
    return (
      <View style={styles.container}>
        <Text style={styles.errorText}>{error}</Text>
      </View>
    );
  }

  // Hlavní render komponenty
  return (
    <View style={styles.container}>

      {/* Avatar profilu */}
      <View style={styles.avatarContainer}>
        <View style={styles.avatar}>
          <FontAwesome name="user" size={60} color="#FFF" />
        </View>
      </View>

      {/* Informace o profilu */}
      <View style={styles.infoContainer}>
        <View style={styles.infoRow}>
          <Text style={styles.label}>Login:</Text>
          <Text style={styles.value}>{profile.login}</Text>
        </View>
        <View style={styles.infoRow}>
          <Text style={styles.label}>E-mail:</Text>
          <Text style={styles.value}>{profile.email}</Text>
        </View>
        <View style={styles.infoRow}>
          <Text style={styles.label}>Tel:</Text>
          <Text style={styles.value}>{profile.phone}</Text>
        </View>
        <View style={styles.infoRow}>
          <Text style={styles.label}>Jméno:</Text>
          <Text style={styles.value}>{profile.nickname}</Text>
        </View>
        <View style={styles.infoRow}>
          <Text style={styles.label}>Počet akcí:</Text>
          <Text style={styles.value}>
            {profile.visitorActions.length + profile.workerActions.length}
          </Text>
        </View>
      </View>

      {/* Tlačítka na přepnutí rolí */}
      <View style={styles.roleSwitchContainer}>
        <TouchableOpacity 
          style={[
            styles.roleButton,
            login === "user" && styles.activeRoleButton
          ]} 
          onPress={() => handleRoleSwitch("user")}
        >
          <Text style={[
            styles.roleButtonText,
            login === "user" && styles.activeRoleButtonText
          ]}>Uživatel</Text>
        </TouchableOpacity>
        
        <TouchableOpacity 
          style={[
            styles.roleButton,
            login === "admin" && styles.activeRoleButton
          ]} 
          onPress={() => handleRoleSwitch("admin")}
        >
          <Text style={[
            styles.roleButtonText,
            login === "admin" && styles.activeRoleButtonText
          ]}>Administrátor</Text>
        </TouchableOpacity>
      </View>

      {/* Tlačítko pro úpravu profilu */}
      <View style={styles.buttonContainer}>
        <TouchableOpacity style={styles.button} onPress={() => router.push('/(profile)/edit')}>
          <Text style={styles.buttonText}>Upravit profil</Text>
        </TouchableOpacity>
      </View>

    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  errorText: {
    color: '#f44336',
    textAlign: 'center',
    margin: 20,
  },
  avatarContainer: {
    alignItems: 'center',
    padding: 20,
    backgroundColor: '#fff',
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  avatar: {
    width: 120,
    height: 120,
    borderRadius: 60,
    backgroundColor: '#673ab7',
    justifyContent: 'center',
    alignItems: 'center',
  },
  infoContainer: {
    backgroundColor: '#fff',
    marginTop: 20,
    paddingHorizontal: 20,
    paddingVertical: 10,
  },
  infoRow: {
    flexDirection: 'row',
    paddingVertical: 15,
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  label: {
    flex: 1,
    fontWeight: '600',
    color: '#666',
  },
  value: {
    flex: 2,
    color: '#333',
  },
  roleSwitchContainer: {
    flexDirection: 'row',
    padding: 20,
    gap: 10,
  },
  roleButton: {
    flex: 1,
    padding: 15,
    borderRadius: 8,
    alignItems: 'center',
    backgroundColor: '#fff',
    borderWidth: 1,
    borderColor: '#2196f3',
  },
  activeRoleButton: {
    backgroundColor: '#2196f3',
  },
  roleButtonText: {
    color: '#2196f3',
    fontSize: 16,
    fontWeight: '600',
  },
  activeRoleButtonText: {
    color: '#fff',
  },
  buttonContainer: {
    padding: 20,
  },
  button: {
    backgroundColor: '#2196f3',
    padding: 15,
    borderRadius: 8,
    alignItems: 'center',
  },
  buttonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
  },
    loadingOverlay: {
    position: 'absolute',
    left: 0,
    right: 0,
    top: 0,
    bottom: 0,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
