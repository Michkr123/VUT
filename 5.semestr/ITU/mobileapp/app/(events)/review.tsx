// ITU Projekt
// Mobilní aplikace - Akce pro lidi
// review.tsx - Vytváření recenzí 
// Autor: Petr Hron (xhronpe00)

import React, { useState, useEffect } from 'react';
import { 
  View, 
  Text, 
  StyleSheet, 
  TextInput, 
  TouchableOpacity, 
  Alert,
  KeyboardAvoidingView,
  Platform,
  ScrollView
} from 'react-native';
import { Rating } from 'react-native-ratings';
import { useLocalSearchParams, router } from 'expo-router';
import { BACKEND_IP, BACKEND_PORT } from '@/backend_address';
import { useUser } from '@/context/UserContext';
import { Review } from '@/types/Review';

export default function ReviewScreen() {
  const { eventId, reviewId } = useLocalSearchParams();
  const { login } = useUser();
  const [isLoading, setIsLoading] = useState(false);
  const [review, setReview] = useState({
    rating: 5,
    comment: '',
    username: ''
  });

  // Získání existující recenze pokud upravujeme
  useEffect(() => {
    const fetchProfile = async () => {
      try {
        const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/profiles/${login}`);
        const data = await response.json();
        setReview(prev => ({ ...prev, username: data.nickname }));
      } catch (error) {
        console.error("Error fetching profile:", error);
      }
    };

    const fetchReview = async () => {
      if (reviewId) {
        try {
          const response = await fetch(`http://${BACKEND_IP}:${BACKEND_PORT}/events/${eventId}`);
          const data = await response.json();
          const existingReview = data.reviews.find((r: Review) => r.id === Number(reviewId));
          if (existingReview) {
            setReview({
              rating: existingReview.rating,
              comment: existingReview.comment,
              username: existingReview.username
            });
          }
        } catch (error) {
          console.error("Error fetching review:", error);
        }
      }
    };

    fetchProfile();
    if (reviewId) {
      fetchReview();
    }
  }, [eventId, reviewId, login]);

  // Uložení recenze
  const handleSubmit = async () => {
    try {
      setIsLoading(true);
      const url = reviewId 
        ? `http://${BACKEND_IP}:${BACKEND_PORT}/events/${eventId}/reviews/${reviewId}`
        : `http://${BACKEND_IP}:${BACKEND_PORT}/events/${eventId}/reviews`;
      
      const response = await fetch(url, {
        method: reviewId ? 'PUT' : 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(review),
      });

      if (response.ok) {
        router.back();
      } else {
        Alert.alert('Chyba', 'Nepodařilo se uložit recenzi.');
      }
    } catch (error) {
      console.error('Error submitting review:', error);
      Alert.alert('Chyba', 'Nepodařilo se uložit recenzi.');
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

          {/* Hodnocení recenze */}
          <Text style={styles.label}>Hodnocení</Text>
          <View style={styles.ratingContainer}>
            <Rating
              type="custom"
              ratingCount={5}
              startingValue={review.rating}
              imageSize={40}
              fractions={2}
              onFinishRating={(rating: number) => setReview(prev => ({ ...prev, rating }))}
              style={styles.rating}
              tintColor="#f5f5f5" // Stejné jako barva pozadí karty
              ratingBackgroundColor="#ddd"
            />
            <Text style={styles.ratingText}>
              {review.rating.toFixed(1)} / 5
            </Text>
          </View>

          {/* Komentář recenze */}
          <Text style={styles.label}>Komentář</Text>
          <TextInput
            style={styles.input}
            value={review.comment}
            onChangeText={(text) => setReview(prev => ({ ...prev, comment: text }))}
            multiline
            numberOfLines={4}
            placeholder="Napište svůj komentář..."
            textAlignVertical="top"
          />

          {/* Odesílací tlačítka */}
          <View style={styles.buttonsContainer}>
            <TouchableOpacity 
              style={[styles.button, styles.cancelButton]}
              onPress={() => router.back()}
            >
              <Text style={styles.cancelButtonText}>Zrušit</Text>
            </TouchableOpacity>

            <TouchableOpacity 
              style={[styles.button, styles.submitButton]}
              onPress={handleSubmit}
              disabled={isLoading}
            >
              <Text style={styles.submitButtonText}>
                {reviewId ? 'Upravit' : 'Přidat'} recenzi
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
    marginBottom: 10,
    color: '#333',
  },
  starsContainer: {
    flexDirection: 'row',
    marginBottom: 20,
    justifyContent: 'center',
  },
  star: {
    fontSize: 40,
    padding: 5,
  },
  input: {
    backgroundColor: '#fff',
    borderRadius: 8,
    padding: 12,
    fontSize: 16,
    minHeight: 100,
    marginBottom: 20,
    borderWidth: 1,
    borderColor: '#ddd',
  },
  buttonsContainer: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    gap: 10,
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
  ratingContainer: {
    alignItems: 'center',
    marginBottom: 20,
  },
  rating: {
    marginBottom: 10,
  },
  ratingText: {
    fontSize: 16,
    color: '#666',
    marginTop: 5,
  },
});
