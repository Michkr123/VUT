// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// edit.tsx - Úprava profilu uživatele
// Autoři: Petr Hron (xhronpe00)
//         Marek Antoňů (xanton07)

import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { useUser } from '@/context/UserContext';
import { Profile } from '@/types/Profile';
import { router } from 'expo-router';
import React, { useEffect, useState } from 'react';
import {
  ActivityIndicator,
  Alert,
  KeyboardAvoidingView,
  Platform,
  ScrollView,
  StyleSheet,
  Text,
  TextInput,
  TouchableOpacity,
  View
} from 'react-native';

export default function EditProfileScreen() {
  const { login } = useUser();
  const [isLoading, setIsLoading] = useState(true);
  const [isSaving, setIsSaving] = useState(false);
  const [profile, setProfile] = useState<Profile | null>(null);
  const [formData, setFormData] = useState({
    email: '',
    phone: '',
    nickname: ''
  });

  // Úvodní načtení profilových dat
  useEffect(() => {
    fetchProfile();
  }, []);

  // Načte profil a uloží do proměnných
  const fetchProfile = async () => {
    try {
      setIsLoading(true);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`);
      if (!response.ok) {
        throw new Error('Failed to fetch profile');
      }
      const data = await response.json();
      setProfile(data);
      setFormData({
        email: data.email,
        phone: data.phone,
        nickname: data.nickname
      });
    } catch (error) {
      console.error('Error fetching profile:', error);
      Alert.alert('Chyba', 'Nepodařilo se načíst profil');
    } finally {
      setIsLoading(false);
    }
  };

  // Uložení upraveného profilu
  const handleSave = async () => {
    try {
      setIsSaving(true);
      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          ...formData,
          // Uchování existujících uživatelem zaregistrovaných akcí
          workerActions: profile?.workerActions || [],
          visitorActions: profile?.visitorActions || []
        }),
      });

      if (!response.ok) {
        throw new Error('Failed to update profile');
      }

      Alert.alert(
        'Úspěch',
        'Profil byl úspěšně aktualizován',
        [
          {
            text: 'OK',
            onPress: () => router.back()
          }
        ]
      );
    } catch (error) {
      console.error('Error updating profile:', error);
      Alert.alert('Chyba', 'Nepodařilo se aktualizovat profil');
    } finally {
      setIsSaving(false);
    }
  };

  // Pokud načítáme profil zobrazíme načítací kolečko
  if (isLoading) {
    return (
      <View style={styles.centerContainer}>
        <ActivityIndicator size="large" color="#2196f3" />
      </View>
    );
  }

  return (
    <KeyboardAvoidingView 
      behavior={Platform.OS === "ios" ? "padding" : "height"}
      style={styles.container}
    >
      <ScrollView style={styles.scrollView}>
        <View style={styles.content}>

          {/* Zobrazení loginu (role) uživatele, pouze pro čtení */}
          <View style={styles.inputGroup}>
            <Text style={styles.label}>Login</Text>
            <View style={styles.readOnlyInput}>
              <Text style={styles.readOnlyText}>{login}</Text>
            </View>
          </View>

          {/* Email */}
          <View style={styles.inputGroup}>
            <Text style={styles.label}>E-mail</Text>
            <TextInput
              style={styles.input}
              value={formData.email}
              onChangeText={(text) => setFormData(prev => ({ ...prev, email: text }))}
              keyboardType="email-address"
              autoCapitalize="none"
              placeholder="Zadejte e-mail"
            />
          </View>

          {/* Telefon */}
          <View style={styles.inputGroup}>
            <Text style={styles.label}>Telefon</Text>
            <TextInput
              style={styles.input}
              value={formData.phone}
              onChangeText={(text) => setFormData(prev => ({ ...prev, phone: text }))}
              keyboardType="phone-pad"
              placeholder="Zadejte telefonní číslo"
            />
          </View>

          {/* Přezdívka */}
          <View style={styles.inputGroup}>
            <Text style={styles.label}>Jméno</Text>
            <TextInput
              style={styles.input}
              value={formData.nickname}
              onChangeText={(text) => setFormData(prev => ({ ...prev, nickname: text }))}
              placeholder="Zadejte přezdívku"
            />
          </View>

          {/* Tlačítka operací */}
          <View style={styles.buttonContainer}>
            <TouchableOpacity
              style={[styles.button, styles.cancelButton]}
              onPress={() => router.back()}
              disabled={isSaving}
            >
              <Text style={styles.cancelButtonText}>Zpět</Text>
            </TouchableOpacity>
            
            <TouchableOpacity
              style={[styles.button, styles.saveButton]}
              onPress={handleSave}
              disabled={isSaving}
            >
              {isSaving ? (
                <ActivityIndicator color="#fff" size="small" />
              ) : (
                <Text style={styles.saveButtonText}>Uložit</Text>
              )}
            </TouchableOpacity>
          </View>

        </View>
      </ScrollView>
    </KeyboardAvoidingView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  centerContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  scrollView: {
    flex: 1,
  },
  content: {
    padding: 20,
  },
  inputGroup: {
    marginBottom: 20,
  },
  label: {
    fontSize: 16,
    fontWeight: '600',
    marginBottom: 8,
    color: '#333',
  },
  input: {
    backgroundColor: '#fff',
    paddingHorizontal: 15,
    paddingVertical: 12,
    borderRadius: 8,
    borderWidth: 1,
    borderColor: '#ddd',
    fontSize: 16,
  },
  readOnlyInput: {
    backgroundColor: '#f0f0f0',
    paddingHorizontal: 15,
    paddingVertical: 12,
    borderRadius: 8,
    borderWidth: 1,
    borderColor: '#ddd',
  },
  readOnlyText: {
    fontSize: 16,
    color: '#666',
  },
  buttonContainer: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    gap: 10,
    marginTop: 20,
  },
  button: {
    flex: 1,
    padding: 15,
    borderRadius: 8,
    alignItems: 'center',
    justifyContent: 'center',
  },
  saveButton: {
    backgroundColor: '#2196f3',
  },
  saveButtonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
  },
  cancelButton: {
    backgroundColor: '#fff',
    borderWidth: 1,
    borderColor: '#ddd',
  },
  cancelButtonText: {
    color: '#666',
    fontSize: 16,
    fontWeight: '600',
  },
});
