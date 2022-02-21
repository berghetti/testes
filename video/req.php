<?php

function get_next_video( $index )
{
  $videos = array();

  foreach ( glob("videos/*.mp4") as $filename )
    {
      $videos[] = $filename;
    }

  $total_videos = count( $videos );

  if ( $index >= $total_videos )
    $index = 0;

  $video = $videos[$index];
  $index = ( $index + 1 ) % $total_videos; // new index to client
  $ret = array('index' => $index, 'video' => $video );

  return json_encode( $ret );
}

echo get_next_video( $_GET['id'] );
