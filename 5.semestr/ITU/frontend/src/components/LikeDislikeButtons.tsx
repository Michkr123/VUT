/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Michálek Kryštof (xmicha94)
Description: Komponenta pro likes a dislikes.
*/

import React, { useState } from 'react';
import { Button } from '@mui/material';

interface LikeDislikeButtonsProps {
  eventId: number;  // ID události
  reviewId: number;  // ID recenze
  initialLikes: string[];  // Počáteční seznam uživatelů, kteří dali like
  initialDislikes: string[];  // Počáteční seznam uživatelů, kteří dali dislike
  userLogin: string;  // Login aktuálního uživatele
}

const LikeDislikeButtons: React.FC<LikeDislikeButtonsProps> = ({ eventId, reviewId, initialLikes, initialDislikes, userLogin }) => {
  // Stav pro počty like/dislike a zda uživatel kliknul
  const [likeCount, setLikeCount] = useState(initialLikes.length);
  const [dislikeCount, setDislikeCount] = useState(initialDislikes.length);
  const [liked, setLiked] = useState(initialLikes.includes(userLogin)); 
  const [disliked, setDisliked] = useState(initialDislikes.includes(userLogin));  

  // Funkce pro zpracování kliknutí na Like
  const handleLike = async () => {
    const action = liked ? 'decrement' : 'increment'; 
    setLiked(!liked);  
    setLikeCount(likeCount + (liked ? -1 : 1));  

    // Pokud uživatel dal dislike, zrušíme ho
    if (disliked) {
      setDisliked(false);
      setDislikeCount(dislikeCount - 1);
      await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/dislike`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ login: userLogin }),
      });
    }

    // Odeslání požadavku pro like
    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/like`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ login: userLogin }),  
    });
  };

  // Funkce pro zpracování kliknutí na Dislike
  const handleDislike = async () => {
    const action = disliked ? 'decrement' : 'increment'; 
    setDisliked(!disliked);  
    setDislikeCount(dislikeCount + (disliked ? -1 : 1)); 

    // Pokud uživatel dal like, zrušíme ho
    if (liked) {
      setLiked(false);
      setLikeCount(likeCount - 1);
      await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/like`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ login: userLogin }), 
      });
    }

    // Odeslání požadavku pro dislike
    await fetch(`http://localhost:5000/events/${eventId}/reviews/${reviewId}/dislike`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ login: userLogin }),  
    });
  };

  return (
    <div className="flex gap-2">
      {/* Tlačítko pro Like */}
      <Button
        variant="contained"
        color={liked ? "primary" : "default"}
        onClick={handleLike}
        style={{ backgroundColor: liked ? 'green' : 'gray' }}  // Zelené pro liked, šedé pro default
      >
        {likeCount} Like
      </Button>

      {/* Tlačítko pro Dislike */}
      <Button
        variant="contained"
        color={disliked ? "secondary" : "default"}
        onClick={handleDislike}
        style={{ backgroundColor: disliked ? 'red' : 'gray' }}  // Červené pro disliked, šedé pro default
      >
        {dislikeCount} Dislike
      </Button>
    </div>
  );
};

export default LikeDislikeButtons;
