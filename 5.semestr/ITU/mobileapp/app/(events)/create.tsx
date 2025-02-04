// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// create.tsx - Vytváření akcí
// Autor: Marek Antoňů (xanton07)

import React, { useState } from 'react';
import { 
  View, 
  Text, 
  StyleSheet, 
  TextInput, 
  TouchableOpacity, 
  ScrollView,
  Alert,
  Image,
  Platform,
  KeyboardAvoidingView,
} from 'react-native';
import { router } from 'expo-router';
import * as ImagePicker from 'expo-image-picker';
import { Picker } from '@react-native-picker/picker';
import DateTimePicker from '@react-native-community/datetimepicker';
import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { EventCategory, Region } from '@/types/Event';

// Názvy a hodnoty pro výběr kategorie
const EVENT_CATEGORIES = [
  { label: 'Zábava', value: 'entertainment' },
  { label: 'Vzdělání', value: 'education' },
  { label: 'Pro dospělé', value: 'for_adults' },
  { label: 'Pro děti', value: 'for_children' },
  { label: 'Workshop', value: 'workshop' },
  { label: 'Výstava', value: 'exposition' },
];

// Názvy a hodnoty pro výběr kraje
const REGIONS = [
  { label: 'Praha', value: 'praha' },
  { label: 'Středočeský', value: 'stredocesky' },
  { label: 'Jihočeský', value: 'jihocesky' },
  { label: 'Plzeňský', value: 'plzensky' },
  { label: 'Karlovarský', value: 'karlovarsky' },
  { label: 'Ústecký', value: 'ustecky' },
  { label: 'Liberecký', value: 'liberecky' },
  { label: 'Královéhradecký', value: 'kralovehradecky' },
  { label: 'Pardubický', value: 'pardubicky' },
  { label: 'Vysočina', value: 'vysocina' },
  { label: 'Jihomoravský', value: 'jihomoravsky' },
  { label: 'Olomoucký', value: 'olomoucky' },
  { label: 'Moravskoslezský', value: 'moravskoslezsky' },
  { label: 'Zlínský', value: 'zlinsky' },
];

export default function CreateEventScreen() {
  const [formData, setFormData] = useState({
    name: '',
    description: '',
    date_of_event: new Date(),
    event_category: '',
    address: '',
    city: '',
    region: '',
    organizer: '',
    image: '',
  });
  const [showDatePicker, setShowDatePicker] = useState(false);
  const [isLoading, setIsLoading] = useState(false);

  // Získání obrázku z galerie
  const pickImage = async () => {
    const result = await ImagePicker.launchImageLibraryAsync({
      mediaTypes: ImagePicker.MediaTypeOptions.Images,
      allowsEditing: true,
      aspect: [16, 9],
      quality: 0.5,
      base64: true,
    });

    if (!result.canceled && result.assets[0].base64) {
      setFormData(prev => ({
        ...prev,
        image: `data:image/jpeg;base64,${result.assets[0].base64}`
      }));
    }
  };

  // Volání výběru data
  const handleDateChange = (event: any, selectedDate?: Date) => {
    setShowDatePicker(false);
    if (selectedDate) {
      setFormData(prev => ({ ...prev, date_of_event: selectedDate }));
    }
  };

  // Vytvoření nové akce
  const handleSubmit = async () => {
    try {
      setIsLoading(true);
      const submitData = {
        ...formData,
        date_of_event: formData.date_of_event.toISOString().split('T')[0],
      };

      const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(submitData),
      });

      if (response.ok) {
        Alert.alert('Úspěch', 'Událost byla úspěšně vytvořena', [
          { text: 'OK', onPress: () => router.back() }
        ]);
      } else {
        Alert.alert('Chyba', 'Nepodařilo se vytvořit událost');
      }
    } catch (error) {
      console.error('Error creating event:', error);
      Alert.alert('Chyba', 'Nepodařilo se vytvořit událost');
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <KeyboardAvoidingView 
      behavior={Platform.OS === "ios" ? "padding" : "height"}
      style={styles.container}
    >
      <ScrollView style={styles.scrollView}>
        <View style={styles.content}>
          {/* Výběr obrázku */}
          <View style={styles.imageContainer}>
            {formData.image ? (
              <TouchableOpacity onPress={pickImage}>
                <Image
                  source={{ uri: formData.image }}
                  style={styles.image}
                />
              </TouchableOpacity>
            ) : (
              <TouchableOpacity 
                style={styles.imagePlaceholder}
                onPress={pickImage}
              >
                <Text style={styles.imagePlaceholderText}>Nahrát obrázek</Text>
              </TouchableOpacity>
            )}
          </View>

          {/* Vstup pro název */}
          <Text style={styles.label}>Název akce</Text>
          <TextInput
            style={styles.input}
            value={formData.name}
            onChangeText={(text) => setFormData(prev => ({ ...prev, name: text }))}
            placeholder="Zadejte název události"
          />

          {/* Výběr kategorie */}
          <Text style={styles.label}>Typ akce</Text>
          <View style={styles.pickerContainer}>
            <Picker
              selectedValue={formData.event_category}
              onValueChange={(value) => setFormData(prev => ({ ...prev, event_category: value as EventCategory }))}
            >
              {EVENT_CATEGORIES.map(category => (
                <Picker.Item 
                  key={category.value} 
                  label={category.label} 
                  value={category.value} 
                />
              ))}
            </Picker>
          </View>

          {/* Vstup pro organizátora */}
          <Text style={styles.label}>Organizátor</Text>
          <TextInput
            style={styles.input}
            value={formData.organizer}
            onChangeText={(text) => setFormData(prev => ({ ...prev, organizer: text }))}
            placeholder="Zadejte jméno organizátora"
          />

          {/* Výběr kraje */}
          <Text style={styles.label}>Kraj</Text>
          <View style={styles.pickerContainer}>
            <Picker
              selectedValue={formData.region}
              onValueChange={(value) => setFormData(prev => ({ ...prev, region: value as Region }))}
            >
              {REGIONS.map(region => (
                <Picker.Item 
                  key={region.value} 
                  label={region.label} 
                  value={region.value} 
                />
              ))}
            </Picker>
          </View>

          {/* Vstup pro město */}
          <Text style={styles.label}>Město</Text>
          <TextInput
            style={styles.input}
            value={formData.city}
            onChangeText={(text) => setFormData(prev => ({ ...prev, city: text }))}
            placeholder="Zadejte město"
          />

          {/* Vstup pro adresu */}
          <Text style={styles.label}>Adresa</Text>
          <TextInput
            style={styles.input}
            value={formData.address}
            onChangeText={(text) => setFormData(prev => ({ ...prev, address: text }))}
            placeholder="Zadejte adresu"
          />

          {/* Výběr data konání */}
          <Text style={styles.label}>Datum konání</Text>
          <TouchableOpacity 
            style={styles.dateButton}
            onPress={() => setShowDatePicker(true)}
          >
            <Text style={styles.dateButtonText}>
              {formData.date_of_event.toLocaleDateString()}
            </Text>
          </TouchableOpacity>

          {showDatePicker && (
            <DateTimePicker
              value={formData.date_of_event}
              mode="date"
              display="default"
              onChange={handleDateChange}
              minimumDate={new Date()}
            />
          )}

          {/* Vstup pro popis */}
          <Text style={styles.label}>Popis</Text>
          <TextInput
            style={[styles.input, styles.textArea]}
            value={formData.description}
            onChangeText={(text) => setFormData(prev => ({ ...prev, description: text }))}
            placeholder="Zadejte popis události"
            multiline
            numberOfLines={4}
          />

          {/* Odesílací tlačítka */}
          <View style={styles.buttonContainer}>
            <TouchableOpacity
              style={[styles.button, styles.cancelButton]}
              onPress={() => router.back()}
              disabled={isLoading}
            >
              <Text style={styles.cancelButtonText}>Zrušit</Text>
            </TouchableOpacity>

            <TouchableOpacity
              style={[styles.button, styles.submitButton]}
              onPress={handleSubmit}
              disabled={isLoading}
            >
              <Text style={styles.submitButtonText}>
                {isLoading ? 'Vytváření...' : 'Vytvořit událost'}
              </Text>
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
  scrollView: {
    flex: 1,
  },
  content: {
    padding: 20,
  },
  label: {
    fontSize: 16,
    fontWeight: '600',
    marginBottom: 8,
    color: '#333',
  },
  imageContainer: {
    marginBottom: 20,
  },
  image: {
    width: '100%',
    height: 200,
    borderRadius: 8,
  },
  imagePlaceholder: {
    width: '100%',
    height: 200,
    backgroundColor: '#e0e0e0',
    borderRadius: 8,
    justifyContent: 'center',
    alignItems: 'center',
  },
  imagePlaceholderText: {
    color: '#666',
    fontSize: 16,
  },
  input: {
    backgroundColor: '#fff',
    padding: 12,
    borderRadius: 8,
    marginBottom: 16,
    fontSize: 16,
    borderWidth: 1,
    borderColor: '#ddd',
  },
  textArea: {
    minHeight: 100,
    textAlignVertical: 'top',
  },
  pickerContainer: {
    backgroundColor: '#fff',
    borderRadius: 8,
    marginBottom: 16,
    borderWidth: 1,
    borderColor: '#ddd',
  },
  dateButton: {
    backgroundColor: '#fff',
    padding: 12,
    borderRadius: 8,
    marginBottom: 16,
    borderWidth: 1,
    borderColor: '#ddd',
  },
  dateButtonText: {
    fontSize: 16,
    color: '#333',
  },
  buttonContainer: {
    flexDirection: 'row',
    gap: 10,
    marginTop: 20,
  },
  button: {
    flex: 1,
    padding: 15,
    borderRadius: 8,
    alignItems: 'center',
  },
  submitButton: {
    backgroundColor: '#2196f3',
  },
  submitButtonText: {
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
